/*-- Forge --*/
//please do not make fun of my awful code, i get it - its bad.

#strict

#include DOOR
#include BAS4
#include CXEC

local Forgebase;
local Mat;

local Prod;

local Producing;
local Forging;

func CanProduce(){return(!Producing && Forgebase != 0);}
func IsProducing(){return(Producing && !Forging);}

local Worker;

local x;
local y;

func Initialize() {
Producing = false;
Forging=false;

if(FindObject2(Find_ID(FALW))){
	Forgebase = FindObject2(Find_ID(FALW));
	return(0);
}
else{
	Forgebase = 0;
	return(0);
}
}

func ContextForge(object pByObject){
	[$CtxForge$|Image=CXCN|Condition=CanProduce]
		CreateMenu(CXCN,pByObject,this(),4,"$MsgNoUseable$");
		for(var i = 1; i < GetLength(LocalN("ForgeableIDs", Forgebase)); i++){
			if(i%2==0) continue;
			AddMenuItem("$ForgeUseMat$: %s","MatSelected",LocalN("ForgeableIDs", Forgebase)[i],pByObject,0,pByObject);
		}
}

func ContextStartContinue(pByObject){
	[$CtxCont$|Image=CXCN|Condition=IsProducing]
	if(pByObject == Worker){
		y = 0;
		x = GetComponent(,y,,Prod);
	}else{
		y -= 1;
	}
	Continue(pByObject);
}

func Continue(pByObject){
	
	//very big fat algorhythm for finding out if any components are still needed
	//very obese

	if(!Forging){
		
	x = GetComponent(,y,,Prod);
	//y++;
	
	var Amt = GetComponent(x, , , Prod);
	if(x == DUMM) x = Mat;
	if(!Producing) return(1);
	
	if(x != 0){
		DebugLog("Forge has %v %s , requires %v. Can do: %v",ContentsCount(x, this()), GetName(,x), Amt, ContentsCount(x, this()) < Amt);
		if(ContentsCount(x, this()) < Amt){
			AddCommand(pByObject, "Wait", , , , , 15);
				if(GetCategory(FindObject2(Find_ID(x))) & C4D_Vehicle()){
					Message("$Move$",pByObject,GetName(,x), Amt);
					AddCommand(pByObject, "Call",this(), , , , , "Continue", 3);
					for(var i = 0; i < Amt; i++){
					AddCommand(pByObject, "Enter", this());
					AddCommand(pByObject, "PushTo", FindObject2(Find_ID(x)), , , this(), , , 3);
					}
					y++;
					return(0);
				}else{
					Message("$Gather$",pByObject,GetName(,x), Amt);
					AddCommand(pByObject, "Call",this(), , , , , "Continue", 3);
					for(var i = 0; i < Amt; i++){
					AddCommand(pByObject, "Put", this());
					AddCommand(pByObject, "Enter", this());
					AddCommand(pByObject, "Acquire", , , , this(), , x);
					}
					y++;
					return(0);
				}
		}
		y++;
		}
		
		if(Contained(pByObject) == this()){
			if(pByObject == Worker){
			var v;
			for(var i = 0; i < y-1; i++){
				v = GetComponent(,i,,Prod);
				Amt = GetComponent(v, , , Prod);
				if(v == DUMM) v = Mat;
				if(ContentsCount(v, this()) < Amt){
					Message("$GatherFail$",pByObject);
					Sound("CommandFailure", false, pByObject);
					return(0);
				}
			}
			StartWork(pByObject);
			}else{
				Message("$WrongWorker$",pByObject,GetName(Worker));
				Sound("CommandFailure", false, pByObject);
			}
		}
	    else{
		AppendCommand(pByObject, "Exit");
		AppendCommand(pByObject, "Call",this(), , , , , "ContextStartContinue", 3);
		return(0);
		}
	}

		
	}


func ContextScrap(pByObject){
	[$CtxScrap$|Image=CXRL|Condition=IsProducing]
	DebugLog("Forging Interrupted: Scrapped");
	Reset();
}

func FreeWork(){
	SetAction("DoorOpen");
	for(var i = 0; i < ObjectCount2(Find_OCF(OCF_CrewMember), Find_Container(this())); i++){
		if (GetController(FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this()))[i]) == -1){
			SetController( GetOwner(FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this()))[i]), FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this()))[i]);
		}
	}
	
	SetObjectStatus(1, Worker, false);
}

func Reset(){
	Producing = false;
	Forging = false;
	Mat = 0;
	Prod = 0;
	Worker = 0;
	x = 0;
	y = 0;
}

func FreeWorkF(){
	Exit(Worker, -8, 25);
	FreeWork();
}
	
func FinishWork(){
	FreeWork();
	Exit(Worker, -8, 25);
	var NewItem = CreateObject(Prod);
	Enter(this(), NewItem);
	
	var Color;
	var Mass;
	var Speed;
	var Power;
	
	for(var i = 0; i < GetLength(LocalN("ForgeableColors", FindObject2(Find_ID(FALW)))); i++){
		if(Mat == LocalN("ForgeableIDs", FindObject2(Find_ID(FALW)))[i])
			Color = LocalN("ForgeableColors", FindObject2(Find_ID(FALW)))[i];
	}
	
	for(var i = 0; i < GetLength(LocalN("ForgeableMass", FindObject2(Find_ID(FALW)))); i++){
		if(Mat == LocalN("ForgeableIDs", FindObject2(Find_ID(FALW)))[i])
			Mass = LocalN("ForgeableMass", FindObject2(Find_ID(FALW)))[i];
	}
	
		for(var i = 0; i < GetLength(LocalN("ForgeableSpeed", FindObject2(Find_ID(FALW)))); i++){
		if(Mat == LocalN("ForgeableIDs", FindObject2(Find_ID(FALW)))[i])
			Speed = LocalN("ForgeableSpeed", FindObject2(Find_ID(FALW)))[i];
	}
	
		for(var i = 0; i < GetLength(LocalN("ForgeablePower", FindObject2(Find_ID(FALW)))); i++){
		if(Mat == LocalN("ForgeableIDs", FindObject2(Find_ID(FALW)))[i])
			Power = LocalN("ForgeablePower", FindObject2(Find_ID(FALW)))[i];
	}
	
	
	
	LocalN("Color", NewItem) = Color;
	LocalN("Mass", NewItem) = Mass;
	LocalN("Speed", NewItem) = Speed;
	LocalN("Power", NewItem) = Power;
	
	NewItem->FRGUpdate();
	
	Message("$ForgeSuccess$", this(), GetName(,Prod));
	Reset();
}

func MatSelected(idType, pByObject){
	Mat = idType;
	Worker = pByObject;
	MenuProduction(pByObject);
	return(0);
}

private func MenuProduction(pCaller) {
  CreateMenu(CXCN,pCaller,this(),1,"$NoPlrKnowledge$");
  for(var i=0,idKnowledge; idKnowledge=GetPlrKnowledge(GetOwner(pCaller),0,i, C4D_All); ++i)
  {
	if(GetComponent(DUMM, , , idKnowledge) == 0)
		continue;
    AddMenuItem("$Construction$: %s", "Gather", idKnowledge, pCaller, 0, pCaller);
  }
  return(1);
}

func Gather(idType, pCaller){
	Prod = idType;
	Producing = true;
	ContextStartContinue(pCaller);
}

func StartWork(User){	

	//clearing all needed components from the building's contents
	var z;
	z = 1;
	y = 0;
	var Amt;
	var Spec;
	
	var EmergencyItems;
	EmergencyItems = CreateArray(50);
	var EID;
	EID = 0;
	
	while(z != 0){
		z = GetComponent(,y,,Prod);
		Amt = GetComponent(z, , , Prod);
		if(z != 0){
			if(z == DUMM) z = Mat;
			DebugLog("Attempting to remove %v (id %v)", GetName(,z), z);
			Spec = FindObjects(Find_ID(z), Find_Container(this()));
			for(var i = 0; i < Amt; i++){
				if(ContentsCount(z, this()) > 0){
					EmergencyItems[EID] = z;
					EID++;
					RemoveObject(Spec[i]);
				}else{
						DebugLog("Forging Interrupted: Missing Items");
						Reset();
						Message("$MissingItems$", this());
						Sound("Discharge");
						for(var i = 0; EmergencyItems[i] != 0; i++){
							CreateContents(EmergencyItems[i], this(), 1);
						}
						return(0);
				}
			}
		}
		y++;
	}



    Forging = true;
	SetController(-1, User);
	SetObjectStatus(2, User, false);
	SetAction("Forging");
	Worker = User;
}

func RejectEntrance(pIntoObj){
	return(Forging);
}

func Forgefx(){
	if(Forging){
	if (Random(6 * 7)){
		Smoke(+16,-20,8);
		Sound("AnvilWork*");
	}
	if (Random(8*7)) Smoke(10,-24,5+Random(3));
	
	if(GetEnergy() < 1 && FindObject2(Find_ID(ENRG))){
		DebugLog("Forging Interrupted: No Power");
		FreeWork();
		Reset();
		Message("$ForgeFail$", this());
		Sound("Discharge");
	}
	
		for(var i = 0; i < ObjectCount2(Find_OCF(OCF_CrewMember), Find_Container(this())); i++){
		if(FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this()))[i] != Worker){
			Exit(FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this()))[i], -8, 25);
		}
		}
	
	}

	
	
}

