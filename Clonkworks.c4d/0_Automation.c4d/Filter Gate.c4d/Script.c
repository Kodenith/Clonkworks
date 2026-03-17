/*-- Neues Script --*/

#strict 2
#include DRBS
#include IO__

local AdditionalObject;
local Filter;
local FilterPath;

local Out;

private func ShadowID(){ return(1); }
private func MainID(){ return(2); }
protected func IsBuilt() 
{
  return(GetCon() >= 100);
}

func Initialize() {
  Filter = 6;
  SetFilterGraph();
  if(GetID() == FLTG) ChangeDef(FTG1,this());
  return(_inherited());
}

public func SetFilter(pId){
	Filter = pId;
}

func SetFilterGraph(){
	
  var FilterImage = Filter;
  if(Filter == 1) FilterImage = WRKS;
  if(Filter == 2) FilterImage = FNDR;
  if(Filter == 3) FilterImage = L_FU;
  if(Filter == 4) FilterImage = RVLT;
  if(Filter == 5) FilterImage = FLAM;
  if(Filter == 6) FilterImage = MAM5;
  if(Filter == 7) FilterImage = S_DC;
  if(Filter == 8) FilterImage = WIRE;
  
	//outline
  SetGraphics(0,this(),FilterImage,ShadowID(),4);
  SetObjDrawTransform(600,0,((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*1000)/4,0,600,GetDefHeight(GetID())*1000/4+(((GetDefCoreVal("Picture","DefCore",FilterImage,3)/2)*1000)/4)-10000,this(),ShadowID());
  SetClrModulation(RGBa(0,0,1),this(),ShadowID());
  
  //main
  SetGraphics(0,this(),FilterImage,MainID(),4);
  SetObjDrawTransform(500,0,((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*1000)/4,0,500,GetDefHeight(GetID())*1000/4+(((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*1000)/4)-10000,this(),MainID());
}

func ControlUpDouble(){
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
	SetMenuSize(8,,pObj);
	
	//predefined options
	AddMenuItem("$TxtMenu1$: $Temp1$","TryGrabbedFilter",WRKS,pObj,,pObj);
	AddMenuItem("$TxtMenu1$: $Temp2$","TryGrabbedFilter",FNDR,pObj,,pObj);
	AddMenuItem("$TxtMenu1$: $Temp3$","TryGrabbedFilter",L_FU,pObj,,pObj);
	AddMenuItem("$TxtMenu1$: $Temp4$","TryGrabbedFilter",RVLT,pObj,,pObj);
	AddMenuItem("$TxtMenu1$: $Temp5$","TryGrabbedFilter",FLAM,pObj,,pObj);
	AddMenuItem("$TxtMenu1$: $Temp6$","TryGrabbedFilter",MAM5,pObj,,pObj);
	AddMenuItem("$TxtMenu1$: %s","TryGrabbedFilter",S_DC,pObj,,pObj);
	AddMenuItem("$TxtMenu1$: $Temp8$","TryGrabbedFilter",WIRE,pObj,,pObj);
	//empty entries to make space
	for(var i = 0; i < 8; i++){
		AddMenuItem(" ",0,0,pObj);
	}
	
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
	if(Filt == WRKS) Filt = 1;
	if(Filt == FNDR) Filt = 2;
	if(Filt == L_FU) Filt= 3;
	if(Filt == RVLT) Filt = 4;
	if(Filt == FLAM) Filt = 5;
	if(Filt == MAM5) Filt = 6;
	if(Filt == S_DC) Filt = 7;
	if(Filt == WIRE) Filt = 8;
	SetFilter(Filt);
	Sound("Connect");
	
	if(Filter == 1) Message("$TxtFilterSet$",this(),"$Temp1$");
	else if(Filter == 2) Message("$TxtFilterSet$",this(),"$Temp2$");
	else if(Filter == 3) Message("$TxtFilterSet$",this(),"$Temp3$");
	else if(Filter == 4) Message("$TxtFilterSet$",this(),"$Temp4$");
	else if(Filter == 5) Message("$TxtFilterSet$",this(),"$Temp5$");
	else if(Filter == 6) Message("$TxtFilterSet$",this(),"$Temp6$");
	else if(Filter == 7) Message("$TxtFilterSet$",this(),GetName(,S_DC));
	else if(Filter == 8) Message("$TxtFilterSet$",this(),"$Temp8$");
	else Message("$TxtFilterSet$",this(),GetName(,Filter));
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

private func FilterCompatibleItem(pItem){
	var pId = GetID(pItem);
	if(Filter == 6) return(1);
	if(Filter == 1){
		if(pItem->~IsAnvilProduct()) return(1);
		if(pItem->~IsAdvancedProduct()) return(1);
		if(pItem->~IsChemicalProduct()) return(1);
		if(pItem->~IsMixerProduct()) return(1);
	}
	else if(Filter == 2){
		if(pItem->~SmeltResult()) return(1);
	}
	else if(Filter==3){
		if(pItem->~IsREFUItem()) return(1);
	}else if(Filter==4){
		if(GetDefFragile(pId)) return(1);
	}else if(Filter==5){
		if(GetDefContactIncinerate(pId)) return(1);
		if(GetDefBlastIncinerate(pId)) return(1);
	}else if(Filter==7){
		return(RandomX(0,1));
	}else if(Filter==8){
		if(InputActive("Allow")) return(1);
		else return(0);
	}else{
		if(pId == Filter) return(1);
	}
}

func Malfunction(){
	FilterPath = !FilterPath;
	Filter = ERTH;
}

func WireTo(){ return(Filter == 8); }
public func OutputList(){
  return(["OnMove"]);
}

public func OutputActive(){
	return(Out);
}

public func InputList(){
  if(Filter == 8)
  return(["Allow"]);
}

public func TickOut(){
	Out = 1;
	ScheduleCall(this,"StopTickOut",1);
}

public func StopTickOut(){
	Out = 0;
}