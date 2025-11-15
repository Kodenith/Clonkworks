/*-- Neues Script --*/

#strict 2
local EssenceList;
local EssenceListUnique;

func Initialize() {
  AddEffect("Vial",this(),1,0,this());
  return(1);
}

func Hit(){
	if(!Random(3)) Sound("Crystal*");
}

func FxVialInfo(object pTarget, int iEffectNumber){
	return(Format("$TxtInfo$",EssenceList[0],GetName(,EssenceList[0]),EssenceList[1],GetName(,EssenceList[1]),EssenceList[2],GetName(,EssenceList[2]),EssenceList[3],GetName(,EssenceList[3])));
}

public func SetData(List){
	EssenceList = List;
	ArrayRemoveDuplicates(List);
	EssenceListUnique = List;
	
	var Color = MixColor(DefinitionCall(EssenceList[0],"EssenceInfo","Color"),MixColor(DefinitionCall(EssenceList[1],"EssenceInfo","Color"),MixColor(DefinitionCall(EssenceList[2],"EssenceInfo","Color"),DefinitionCall(EssenceList[3],"EssenceInfo","Color"))));
	SetColorDw(Color);
}

public func Activate(pClonk){
	[$TxtDrink$]
	pClonk->Drink(this());
	pClonk->Sound("Drink");

	//exiting and doing some commands before erasure
	SetCategory(1);
	Exit(this());
	SetPosition(0,0);
	SetClrModulation(RGBa(0,0,1,255));
	if(!EssenceList){
		RemoveObject();
		return(1);
	}
	
	ScheduleCall(this(),"DoEssenceEffect",36*3,0,pClonk);
	return(1);
}

public func DoEssenceEffect(pClonk){
	if(!pClonk) return(0);
	if(GetEffectCount("EssencePhysicals",pClonk) > 0){
		Kill(pClonk);
		Schedule("Explode(15)",36,0,pClonk);
		return(0);
	}
	//for now i set it to a permanent number because apparently it breaks if its not
	AddEffect("EssencePhysicals",pClonk,50,36/2,,GetID(),LocalN("EssenceList",this()),LocalN("EssenceListUnique",this()),GetColorDw());
	
	Schedule("RemoveObject()",5,,this());
}

//essence effect
public func FxEssencePhysicalsStart(pTarget, iEffectNumber, iTemp, List,UniqueList,Color){
	EffectVar(0,pTarget,iEffectNumber) = List;
	EffectVar(1,pTarget,iEffectNumber) = UniqueList;
	EffectVar(2,pTarget,iEffectNumber) = Color;
	
	var nList = List;
	//if(!nList) return(-1);
	var Energy, Breath, Walk, Jump, Scale, Hangle, Dig, Swim, Throw, Push, Fight;
	for (var i in nList) {
        // Energy
        if (DefinitionCall(i, "EssenceInfo", "Energy") != 0) {
            Energy += DefinitionCall(i, "EssenceInfo", "Energy");
        }

        // Breath
        if (DefinitionCall(i, "EssenceInfo", "Breath") != 0) {
            Breath += DefinitionCall(i, "EssenceInfo", "Breath");
        }

        // Walk
        if (DefinitionCall(i, "EssenceInfo", "Walk") != 0) {
            Walk += DefinitionCall(i, "EssenceInfo", "Walk");
        }

        // Jump
        if (DefinitionCall(i, "EssenceInfo", "Jump") != 0) {
            Jump += DefinitionCall(i, "EssenceInfo", "Jump");
        }

        // Scale
        if (DefinitionCall(i, "EssenceInfo", "Scale") != 0) {
            Scale += DefinitionCall(i, "EssenceInfo", "Scale");
        }

        // Hangle
        if (DefinitionCall(i, "EssenceInfo", "Hangle") != 0) {
            Hangle += DefinitionCall(i, "EssenceInfo", "Hangle");
        }

        // Dig
        if (DefinitionCall(i, "EssenceInfo", "Dig") != 0) {
            Dig += DefinitionCall(i, "EssenceInfo", "Dig");
        }

        // Swim
        if (DefinitionCall(i, "EssenceInfo", "Swim") != 0) {
            Swim += DefinitionCall(i, "EssenceInfo", "Swim");
        }

        // Throw
        if (DefinitionCall(i, "EssenceInfo", "Throw") != 0) {
            Throw += DefinitionCall(i, "EssenceInfo", "Throw");
        }

        // Push
        if (DefinitionCall(i, "EssenceInfo", "Push") != 0) {
            Push += DefinitionCall(i, "EssenceInfo", "Push");
        }

        // Fight
        if (DefinitionCall(i, "EssenceInfo", "Fight") != 0) {
            Fight += DefinitionCall(i, "EssenceInfo", "Fight");
        }
    }
	
	SetPhysical("Energy", GetPhysical("Energy", 0, pTarget) + Energy, 3,pTarget);
    SetPhysical("Breath", GetPhysical("Breath", 0, pTarget) + Breath, 3,pTarget);
    SetPhysical("Walk", GetPhysical("Walk", 0, pTarget) + Walk, 3,pTarget);
    SetPhysical("Jump", GetPhysical("Jump", 0, pTarget) + Jump, 3,pTarget);
    SetPhysical("Scale", GetPhysical("Scale", 0, pTarget) + Scale, 3,pTarget);
    SetPhysical("Hangle", GetPhysical("Hangle", 0, pTarget) + Hangle, 3,pTarget);
    SetPhysical("Dig", GetPhysical("Dig", 0, pTarget) + Dig, 3,pTarget);
    SetPhysical("Swim", GetPhysical("Swim", 0, pTarget) + Swim, 3,pTarget);
    SetPhysical("Throw", GetPhysical("Throw", 0, pTarget) + Throw, 3,pTarget);
    SetPhysical("Push", GetPhysical("Push", 0, pTarget) + Push, 3,pTarget);
    SetPhysical("Fight", GetPhysical("Fight", 0, pTarget) + Fight, 3,pTarget);
	
	var nUniqueList = UniqueList;
	for(var j in nUniqueList){
		DefinitionCall(j, "EssenceInfo", "Start",pTarget);
	}
}

public func FxEssencePhysicalsTimer(object pTarget, int iEffectNumber, int iEffectTime){
	pTarget->CreateParticle("PSpark",RandomX(-10,10),7,0,-5,RandomX(25,35),EffectVar(2,pTarget,iEffectNumber),pTarget);
	if(iEffectTime > 36*60) return(-1);
}

public func FxEssencePhysicalsStop(object pTarget, int iEffectNumber, int iReason, bool fTemp){
	var Phys = ["Energy","Breath","Walk","Jump","Scale","Hangle","Dig","Swim","Throw","Push","Fight"];
	for(var p in Phys){
		//DebugLog("InEffect %s: %d",p,GetPhysical(p, 0, pTarget));
		ResetPhysical(pTarget,p);
		//DebugLog("Current %s: %d",p,GetPhysical(p, 0, pTarget));
	}
	
	
	var nUniqueList = EffectVar(1,pTarget,iEffectNumber);
	for(var j in nUniqueList){
		DefinitionCall(j, "EssenceInfo", "Stop",pTarget);
	}
}