
#include "filter.hpp"

#include "../mwbase/environment.hpp"
#include "../mwbase/world.hpp"
#include "../mwbase/journal.hpp"
#include "../mwbase/mechanicsmanager.hpp"

#include "../mwworld/class.hpp"
#include "../mwworld/player.hpp"
#include "../mwworld/containerstore.hpp"
#include "../mwworld/inventorystore.hpp"

#include "../mwmechanics/npcstats.hpp"
#include "../mwmechanics/creaturestats.hpp"
#include "../mwmechanics/magiceffects.hpp"

#include "selectwrapper.hpp"

namespace
{
    std::string toLower (const std::string& name)
    {
        std::string lowerCase;

        std::transform (name.begin(), name.end(), std::back_inserter (lowerCase),
            (int(*)(int)) std::tolower);

        return lowerCase;
    }
}

bool MWDialogue::Filter::testActor (const ESM::DialInfo& info) const
{
    // actor id
    if (!info.mActor.empty())
        if (toLower (info.mActor)!=MWWorld::Class::get (mActor).getId (mActor))
            return false;

    bool isCreature = (mActor.getTypeName() != typeid (ESM::NPC).name());

    // NPC race
    if (!info.mRace.empty())
    {
        if (isCreature)
            return false;

        MWWorld::LiveCellRef<ESM::NPC> *cellRef = mActor.get<ESM::NPC>();

        if (toLower (info.mRace)!=toLower (cellRef->mBase->mRace))
            return false;
    }

    // NPC class
    if (!info.mClass.empty())
    {
        if (isCreature)
            return false;

        MWWorld::LiveCellRef<ESM::NPC> *cellRef = mActor.get<ESM::NPC>();

        if (toLower (info.mClass)!=toLower (cellRef->mBase->mClass))
            return false;
    }

    // NPC faction
    if (!info.mNpcFaction.empty())
    {
        if (isCreature)
            return false;

        MWMechanics::NpcStats& stats = MWWorld::Class::get (mActor).getNpcStats (mActor);
        std::map<std::string, int>::iterator iter = stats.getFactionRanks().find (toLower (info.mNpcFaction));

        if (iter==stats.getFactionRanks().end())
            return false;
            
        // check rank
        if (iter->second < info.mData.mRank)
            return false;
    }

    // Gender
    if (!isCreature)
    {
        MWWorld::LiveCellRef<ESM::NPC>* npc = mActor.get<ESM::NPC>();
        if (info.mData.mGender==(npc->mBase->mFlags & npc->mBase->Female ? 0 : 1))
            return false;
    }
    
    return true;
}

bool MWDialogue::Filter::testPlayer (const ESM::DialInfo& info) const
{
    const MWWorld::Ptr player = MWBase::Environment::get().getWorld()->getPlayer().getPlayer();
    
    // check player faction
    if (!info.mPcFaction.empty())
    {
        MWMechanics::NpcStats& stats = MWWorld::Class::get (player).getNpcStats (player);
        std::map<std::string,int>::iterator iter = stats.getFactionRanks().find (toLower (info.mPcFaction));

        if(iter==stats.getFactionRanks().end())
            return false;

        // check rank
        if (iter->second < info.mData.mPCrank)
            return false;
    }

    // check cell
    if (!info.mCell.empty())
        if (toLower (player.getCell()->mCell->mName) != toLower (info.mCell))
            return false;

    return true;
}

bool MWDialogue::Filter::testSelectStructs (const ESM::DialInfo& info) const
{
    for (std::vector<ESM::DialInfo::SelectStruct>::const_iterator iter (info.mSelects.begin());
        iter != info.mSelects.end(); ++iter)
        if (!testSelectStruct (*iter))
            return false;
            
    return true;
}

bool MWDialogue::Filter::testSelectStruct (const SelectWrapper& select) const
{
    if (select.isNpcOnly() && mActor.getTypeName()!=typeid (ESM::NPC).name())
        return select.isInverted();
    
    switch (select.getType())
    {
        case SelectWrapper::Type_None: return true;
        case SelectWrapper::Type_Integer: return select.selectCompare (getSelectStructInteger (select));
        case SelectWrapper::Type_Numeric: return testSelectStructNumeric (select);
        case SelectWrapper::Type_Boolean: return select.selectCompare (getSelectStructBoolean (select));
    }
    
    return true;
}

bool MWDialogue::Filter::testSelectStructNumeric (const SelectWrapper& select) const
{
    switch (select.getFunction())
    {
        case SelectWrapper::Function_Global:
        
            // internally all globals are float :(
            return select.selectCompare (
                MWBase::Environment::get().getWorld()->getGlobalVariable (select.getName()).mFloat);
        
        case SelectWrapper::Function_Local:
        {
            std::string scriptName = MWWorld::Class::get (mActor).getScript (mActor);

            if (scriptName.empty())
                return false; // no script

            const ESM::Script *script =
                MWBase::Environment::get().getWorld()->getStore().get<ESM::Script>().find (scriptName);

            std::string name = select.getName();

            int i = 0;

            for (; i<static_cast<int> (script->mVarNames.size()); ++i)
                if (script->mVarNames[i]==name)
                    break;

            if (i>=static_cast<int> (script->mVarNames.size()))
                return false; // script does not have a variable of this name

            const MWScript::Locals& locals = mActor.getRefData().getLocals();

            if (i<script->mData.mNumShorts)
                return select.selectCompare (static_cast<int> (locals.mShorts[i]));
                
            i -= script->mData.mNumShorts;

            if (i<script->mData.mNumLongs)
                return select.selectCompare (locals.mLongs[i]);

            i -= script->mData.mNumShorts;

            return select.selectCompare (locals.mFloats.at (i));        
        }
        
        case SelectWrapper::Function_PcHealthPercent:
        {
            MWWorld::Ptr player = MWBase::Environment::get().getWorld()->getPlayer().getPlayer();
        
            float ratio = MWWorld::Class::get (player).getCreatureStats (player).getHealth().getCurrent() / 
                MWWorld::Class::get (player).getCreatureStats (player).getHealth().getModified();
                
            return select.selectCompare (ratio);
        }
            
        case SelectWrapper::Function_PcDynamicStat:
        {
            MWWorld::Ptr player = MWBase::Environment::get().getWorld()->getPlayer().getPlayer();
        
            float value = MWWorld::Class::get (player).getCreatureStats (player).
                getDynamic (select.getArgument()).getCurrent();       
                
            return select.selectCompare (value);
        }

        default:
        
            throw std::runtime_error ("unknown numeric select function");
    }
}

int MWDialogue::Filter::getSelectStructInteger (const SelectWrapper& select) const
{
    MWWorld::Ptr player = MWBase::Environment::get().getWorld()->getPlayer().getPlayer();

    switch (select.getFunction())
    {
        case SelectWrapper::Function_Journal:
        
            return MWBase::Environment::get().getJournal()->getJournalIndex (select.getName());
        
        case SelectWrapper::Function_Item: 
        {        
            MWWorld::ContainerStore& store = MWWorld::Class::get (player).getContainerStore (player);

            int sum = 0;

            std::string name = select.getName();

            for (MWWorld::ContainerStoreIterator iter (store.begin()); iter!=store.end(); ++iter)
                if (toLower(iter->getCellRef().mRefID) == name)
                    sum += iter->getRefData().getCount();
                    
            return sum;
        }

        case SelectWrapper::Function_Dead:

            return MWBase::Environment::get().getMechanicsManager()->countDeaths (select.getName());
        
        case SelectWrapper::Function_Choice:
        
            return mChoice;
        
        case SelectWrapper::Function_AiSetting:
        
            return MWWorld::Class::get (player).getCreatureStats (player).getAiSetting (select.getArgument());
        
        case SelectWrapper::Function_PcAttribute:
        
            return MWWorld::Class::get (player).getCreatureStats (player).
                getAttribute (select.getArgument()).getModified();

        case SelectWrapper::Function_PcSkill:
        
            return static_cast<int> (MWWorld::Class::get (player).
                getNpcStats (player).getSkill (select.getArgument()).getModified());
        
        case SelectWrapper::Function_FriendlyHit:
        {
            int hits = MWWorld::Class::get (mActor).getCreatureStats (mActor).getFriendlyHits();

            return hits>4 ? 4 : hits;            
        }
    
        case SelectWrapper::Function_PcLevel:

            return MWWorld::Class::get (player).getCreatureStats (player).getLevel();

        case SelectWrapper::Function_PcGender:
        {
            MWWorld::LiveCellRef<ESM::NPC> *cellRef = player.get<ESM::NPC>();
        
            return cellRef->mBase->Female ? 0 : 1;
        }
            
        case SelectWrapper::Function_PcClothingModifier:
        {
            MWWorld::InventoryStore& store = MWWorld::Class::get (player).getInventoryStore (player);

            int value = 0;
        
            for (int i=0; i<=15; ++i) // everything except thigns held in hands and amunition
            {
                MWWorld::ContainerStoreIterator slot = store.getSlot (i);
                
                if (slot!=store.end())
                    value += MWWorld::Class::get (*slot).getValue (*slot);
            }
        
            return value;
        }
            
        case SelectWrapper::Function_PcCrimeLevel:
        
            return MWWorld::Class::get (player).getNpcStats (player).getBounty();
            
        default:

            throw std::runtime_error ("unknown integer select function");
    }
}

bool MWDialogue::Filter::getSelectStructBoolean (const SelectWrapper& select) const
{
    MWWorld::Ptr player = MWBase::Environment::get().getWorld()->getPlayer().getPlayer();

    switch (select.getFunction())
    {
        case SelectWrapper::Function_False:
        
            return false;
            
        case SelectWrapper::Function_Id:
        
            return select.getName()==toLower (MWWorld::Class::get (mActor).getId (mActor));
        
        case SelectWrapper::Function_Faction:

            return toLower (mActor.get<ESM::NPC>()->mBase->mFaction)==select.getName();

        case SelectWrapper::Function_Class:

            return toLower (mActor.get<ESM::NPC>()->mBase->mClass)==select.getName();

        case SelectWrapper::Function_Race:

            return toLower (mActor.get<ESM::NPC>()->mBase->mRace)==select.getName();

        case SelectWrapper::Function_Cell:
    
            return toLower (mActor.getCell()->mCell->mName)==select.getName();
    
        case SelectWrapper::Function_SameFaction:

            return MWWorld::Class::get (mActor).getNpcStats (mActor).isSameFaction (
                MWWorld::Class::get (player).getNpcStats (player));
        
        case SelectWrapper::Function_PcCommonDisease:

            return MWWorld::Class::get (player).getCreatureStats (player).hasCommonDisease();
        
        case SelectWrapper::Function_PcBlightDisease:

            return MWWorld::Class::get (player).getCreatureStats (player).hasBlightDisease();
    
        case SelectWrapper::Function_PcCorprus:
        
            return MWWorld::Class::get (player).getCreatureStats (player).
                getMagicEffects().get (132).mMagnitude!=0;
    
        case SelectWrapper::Function_PcExpelled:
        {
            if (MWWorld::Class::get (mActor).getNpcStats (mActor).getFactionRanks().empty())
                return false;
                
            std::string faction =
                MWWorld::Class::get (mActor).getNpcStats (mActor).getFactionRanks().begin()->first;
            
            std::set<std::string>& expelled = MWWorld::Class::get (player).getNpcStats (player).getExpelled();
            
            return expelled.find (faction)!=expelled.end();
        }
        
        case SelectWrapper::Function_PcVampire:
        
            return MWWorld::Class::get (player).getNpcStats (player).isVampire();
        
        case SelectWrapper::Function_TalkedToPc:
        
            return mTalkedToPlayer;

        default:

            throw std::runtime_error ("unknown boolean select function");
    }
}

MWDialogue::Filter::Filter (const MWWorld::Ptr& actor, int choice, bool talkedToPlayer)
: mActor (actor), mChoice (choice), mTalkedToPlayer (talkedToPlayer)
{}

bool MWDialogue::Filter::operator() (const ESM::DialInfo& info) const
{
    return testActor (info) && testPlayer (info) && testSelectStructs (info);
}