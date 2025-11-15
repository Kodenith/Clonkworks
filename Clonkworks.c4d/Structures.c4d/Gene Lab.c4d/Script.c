/*-- Neues Script --*/

#strict 2
#include DOOR
#include BAS9

local pWorker;
local Selected;
local EssenceList;
local Color;

local iter;

private func SoundOpenDoor()
{
  Sound("SteelGate2");
}
  
private func SoundCloseDoor()
{
  Sound("SteelGate2");
}

public func SplicingAvailable(){
	if(pWorker) return(0);
	if(GetAction() == "Process") return(0);
	return(1);
}

public func CanSplice(){
	if(GetLength(FindObjects(Find_ID(SRIG), Find_Action("Filled"), Find_Container(this()))) > 3) return(1);
}

//production
public func ContextBrew(pClonk){
	[$TxtSplice$|Image=CH01|Condition=SplicingAvailable]
	
	//hostile clonks cant use this!
	if(Hostile(GetOwner(),GetOwner(pClonk))){
		return(0);
	}
	
	//is clonk outside? come on in!
	if(Contained(pClonk) != this()){
		AddCommand(pClonk,"Call",this(),pClonk,,,,"ContextBrew");
		AddCommand(pClonk,"Enter",this());
		return(0);
	}
	
	//is someone already working here? dont push in.
	if(pWorker && pWorker != pClonk){
		Sound("Error");
		PlayerMessage(GetOwner(pClonk),"$TxtCannotWork$",this(),GetName(pWorker));
		return(0);
	}
	
	//no dna to work with? get out of heeeree..
	if(!CanSplice()){
		Sound("Error");
		PlayerMessage(GetOwner(pClonk),"$TxtCannotSplice$",this());
		return(0);
	}
	
	pWorker = pClonk;
	Selected = [];
	EssenceSelection(pClonk);
}

public func EssenceSelection(pClonk){
	if(!CanSplice()){
		Selected = [];
		EssenceList = [];
		return(0);
	}
	
	if(!Selected) Selected = [];
	PlayerMessage(GetOwner(pClonk),"$TxtProgress$",,GetLength(Selected));
	
	if(pClonk != pWorker) return(0);
		
	if(GetLength(Selected) >= 4){
		StartProduction(pClonk);
		return(0);
	}
	
	CreateMenu(CH01,pClonk,this(),0,"$TxtCannotSplice$");
	var o,i;
	while(o = Contents(i++,this())){
		if(InArray(o,Selected) != -1) continue;
		if(GetID(o) != SRIG) continue;
		if(GetAction(o) != "Filled") continue;
		AddMenuItem(Format("$TxtAdd$",GetName(,LocalN("Essence",o))), "AddSelection", LocalN("Essence",o),pClonk,,pClonk,,4,o);
	}
	
	pWorker = pClonk;
}

public func StartProduction(pClonk){
	for(var i = 0; i < GetLength(Selected); i++){
		if(!EssenceList) EssenceList = [];
		ArrayAdd(EssenceList,LocalN("Essence",Selected[i]));
	}
	
	CloseMenu(pClonk);
	if(GetLength(EssenceList) < 4) return(0);
	if(!pWorker) return(0);
	iter = 0;
	SetAction("Process");
	Color = MixColor(GetColorDw(Selected[0]),MixColor(GetColorDw(Selected[1]),MixColor(GetColorDw(Selected[2]),GetColorDw(Selected[3]))));
}

func MakeGoo(){
	if(iter > 3){
		var Goo = CreateObject(VIAL);
		Enter(this(),Goo);
		SetColorDw(Color,Goo);
		
		Goo->~SetData(EssenceList);
		
		SetAction("Idle");
		Sound("done");
		
		for(var i in Selected){
			RemoveObject(i);
		}
		
		pWorker = 0;
		Selected = [];
		EssenceList = [];
		return(0);
	}
	
	if(!pWorker){
		SetAction("Idle");
		return(0);
	}
	
	var iX,iY;
	iX = GetVertex(8,0);
	iY = GetVertex(8,1);
	
	CreateParticle("FSpark",iX,iY,0,-20,RandomX(80,100),GetColorDw(Selected[iter]));
	Sound("Chuff");
	iter++;
}

func AddSelection(Essence,pClonk){
	//make sure a syringe like that exists
	var o,i,ok;
	while(o = Contents(i++,this())){
		if(InArray(o,Selected) != -1) continue;
		if(GetID(o) != SRIG) continue;
		if(GetAction(o) != "Filled") continue;
		if(LocalN("Essence",o) == Essence){
			ok = true;
			break;
		}
	}
	
	if(ok){
		ArrayAdd(Selected,o);
		EssenceSelection(pClonk);
	}
}

public func RejectContents(){
	if(pWorker) return(1);
}

//efficient code i totally didnt steal from github
global func MixColor(int col1, int col2){
	var bi = 2;
	var R,G,B,R2,G2,B2,foo;
	SplitRGBaValue(col1,R,G,B,foo);
	SplitRGBaValue(col2,R2,G2,B2,foo);
	
	var endR = bi*R+R2*(5-bi);
	var endG = bi*G+G2*(5-bi);
	var endB = bi*B+B2*(5-bi);
	var scale = Max(Max(endR,endG),endB)/((2*bi*Max(Max(R,G),B)+(2-2*bi)*Max(Max(R2,G2),B2))/2);
	endR = endR/scale;
    endG = endG/scale;
    endB = endB/scale;
	return(RGBa(endR,endG,endB));
}

public func UpdateWorker(){
	if(pWorker){
	if(!GetAlive(pWorker)) pWorker = 0;
	if(GetAction() != "Process"){
		if(GetMenu(pWorker) != CH01) pWorker = 0;
	}
	if(Contained(pWorker) != this()) pWorker = 0;
	}
}

public func GetResearchBase() { return(ADVW); }