/*-- Neues Script --*/

#strict 2
#include DRBS

local AdditionalObject;
local Filter;
local FilterPath;

private func ShadowID(){ return(1); }
private func MainID(){ return(2); }
protected func IsBuilt() 
{
  return(GetCon() >= 100);
}

func Initialize() {
  Filter = GOLD;
  SetFilterGraph();
  if(GetID() == FLTG) ChangeDef(FTG1,this());
  return(_inherited());
}

public func SetFilter(pId){
	Filter = pId;
}

func SetFilterGraph(){
	//outline
  SetGraphics(0,this(),Filter,ShadowID(),4);
  SetObjDrawTransform(600,0,((GetDefCoreVal("Picture","DefCore",Filter,2)/2)*1000)/4,0,600,GetDefHeight(GetID())*1000/4+(((GetDefCoreVal("Picture","DefCore",Filter,3)/2)*1000)/4)-10000,this(),ShadowID());
  SetClrModulation(RGBa(0,0,1),this(),ShadowID());
  
  //main
  SetGraphics(0,this(),Filter,MainID(),4);
  SetObjDrawTransform(500,0,((GetDefCoreVal("Picture","DefCore",Filter,2)/2)*1000)/4,0,500,GetDefHeight(GetID())*1000/4+(((GetDefCoreVal("Picture","DefCore",Filter,2)/2)*1000)/4)-10000,this(),MainID());
}

func ControlDigDouble(){
	[$TxtDeconstruct$]
	Sound("SignalClick");
    CreateObject(CNKT,0,0,GetOwner(Par(0)) );
	RemoveObject();
	return(1);
}

func ControlLeft(pObj){
	[$TxtMenu1$]
	if(GetProcedure(pObj) != "PUSH") return(0);
	if(GetActionTarget(0,pObj) != this());
	CreateMenu(GetID(),pObj,this(),,"error");
	var i,j;
	while(j = GetDefinition(i++,C4D_Object)){
		var ok = false;
		if(GetCategory(,j) & 512) ok = true;
		if(GetCategory(,j) & 1024) ok = true;
		if(GetCategory(,j) & 2048) ok = true;
		if(GetCategory(,j) & 8192) ok = true;
		if(GetCategory(,j) & 16384) ok = true;
		if(GetCategory(,j) & 65536) ok = true;
		if(ok) AddMenuItem("$TxtMenu1$: %s","TryGrabbedFilter",j,pObj,,pObj);
	}
}

func TryGrabbedFilter(Filt,pObj){
	if(GetProcedure(pObj) != "PUSH") return(0);
	if(GetActionTarget(0,pObj) != this());
	SetFilter(Filt);
	Sound("Connect");
	Message("$TxtFilterSet$",this(),GetName(,Filter));
}

public func ContextDoFilter(pClonk){
	[$TxtMenu1$|Image=FLTG|Condition=IsBuilt]
	AddCommand(pClonk,"Call",this(),0,0,0,0,"ControlLeft");
	AddCommand(pClonk,"Grab",this());
}

//changing
func ControlRight(pClonk){
	[$TxtChange$|Image=FLTG]
	if(GetProcedure(pClonk) != "PUSH") return(0);
	if(GetActionTarget(0,pClonk) != this());
	CreateMenu(FLTG,pClonk,this(),4,"error");
	var i,j;
	while(j = GetDefinition(i++,C4D_Structure)){
		//DebugLog("%v",j->~SelectableGate());
		if(j->~SelectableGate()) AddMenuItem("%s","TryChange",j,pClonk,,pClonk,GetDesc(,j),);
	}
}

func TryChange(Filt,pObj){
	if(GetProcedure(pObj) != "PUSH") return(0);
	if(GetActionTarget(0,pObj) != this());
	Sound("Click");
	if(AdditionalObject) RemoveObject(AdditionalObject);
	FilterPath = 0;
	ChangeDef(Filt,this());
	this()->~OnChange();
}

public func ContextDoChange(pClonk){
	[$TxtChange$|Image=FLTG|Condition=IsBuilt]
	AddCommand(pClonk,"Call",this(),0,0,0,0,"ControlRight");
	AddCommand(pClonk,"Grab",this());
}

func Destruction(){
	if(AdditionalObject) RemoveObject(AdditionalObject);
	if(basement) RemoveObject(basement);
}

func GetResearchBase(){
	return(CNVY);
}
func IsAdvancedProduct(){ return(1); }