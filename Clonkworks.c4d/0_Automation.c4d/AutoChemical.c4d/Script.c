/*-- Neues Script --*/

#strict 2
#include CXEC

local KeepItems;

/* GENERAL AUTOMATED PRODUCTION TEMPLATE */

local Parent;
public func RejectConstruction(iX,iY,pBuilder){
	if(!ObjectOnConveyor(pBuilder)){
		Message("$TxtWrongPlacement$",pBuilder);
		pBuilder->Sound("Error");
		return(1);
	}
	
	return(0);
}

protected func Construction(){
	var Look;
	if(Look = FindObject2(Find_Func("IsConveyor"),Find_OnLine(0,0,0,10))){
		Parent = Look;
		LocalN("Extension",Parent) = this();
	}
	else RemoveObject();
	
	return(_inherited());
}

public func GetCDir(){
	if(!Parent) return(0);
	if(LocalN("Speed",Parent) == 0) return(0);
	if(LocalN("Speed",Parent) < 0) return(-1);
	return(1);
}

public func IsBuilt(){ return(GetCon() >= 100); }
protected func BuildNeedsMaterial(){ return(1); }

/* END */

local Product;
local Producing;

protected func Initialize(){
	Product = GUNP;
	return(1);
}

//Quick Array Get For Recipe
public func GetProductComponentArray(){
	var Ar = [];
	var x = 0;
	var i;
	while(i = GetComponent(,x++,,Product)){
		ArrayAdd(Ar,i,true);
	}
	return(Ar);
}

public func GetOrderComponentArray(){
	var Ar = [];
	for(var Od in OrderID){
		var x = 0;
		var i;
		while(i = GetComponent(,x++,,Od)){
			ArrayAdd(Ar,i,true);
		}
	}
	return(Ar);
}

//Collection, based on conveyor direction. i cant have both pipes on the sprite go to waste :)
protected func RejectCollect(idObj,pObj){
	if(GetCDir() == 0) return(1);
	if(pObj->~UnpackTo()){
		Enter(this,pObj);
		pObj->Unpack();
		Sound("Clonk");
		return(1);
	}
	if(GetCDir() == 1){
		if(GetX(pObj) < GetX()-10) return(0);
	}
	if(GetCDir() == -1){
		if(GetX(pObj) > GetX()+10) return(0);
	}
	
	return(1);
}

protected func Collection(pObj,fPut){
	if(fPut) return(1);
	Sound("Clonk");
	
	var Ingr = GetProductComponentArray();
	if(GetEffect("OrderMode",this)){
		Ingr = GetOrderComponentArray();
	}
	if(InArray(GetID(pObj),Ingr) == -1){
		Exit(pObj,20*GetCDir(),GetDefBottom()-GetY());
		return(0);
	}
	
	return(1);
}

//Setting filter. setting one automatically checks all items so they quit.
public func SetFilter(pId,pGrabber){
	if(DefinitionCall(pId,"IsChemicalProduct") != 1) return(0);
	if(pGrabber && (GetAction(pGrabber) != "Push" || GetActionTarget(0,pGrabber) != this())) return(0);
	Product = pId;
	for(var i in FindObjects(Find_Container(this()))){
		Collection(i,0);
	}
	CheckContents();
	if(pGrabber){
		Message("$TxtSet$",this(),GetName(,Product));
		Sound("Click");
		if(GetEffect("OrderMode",this)){
			RemoveEffect("OrderMode",this);
		}
	}
}

func ControlUp(pClonk){
	[$TxtSetFilter$]
	CreateMenu(GetID(),pClonk,this(),1);
	var x,i;
	while(x = GetDefinition(i++,C4D_Object)){
		if(!DefinitionCall(x,"IsChemicalProduct")) continue;
		if(!GetPlrKnowledge(GetController(pClonk),x)) continue;
		AddMenuItem("%s","SetFilter",x,pClonk,0,pClonk);
	}
}

public func ContextSetFilter(pClonk){
	[$TxtSetFilter$|Image=CHM2|Condition=IsBuilt]
	AddCommand(pClonk,"Call",this(),pClonk,0,0,38*999,"ControlUp");
	AddCommand(pClonk,"Grab",this());
}

//Logic
local Sign;
protected func CheckContents(){
	if(!IsBuilt()) return(0);
	
	if(!Sign){
		Sign = CreateObject(PDS_);
	}
	
	if(Parent && !OnFire(Parent)){
		SetPosition(GetX(),GetY(Parent)+5,Sign);
		Sign->SetPic(Product);
	}else{
		Incinerate(); return(0);
	}
	
	if(GetCDir() == 0 || !EnergyCheck(10000)) return(0);
	if(ActIdle()){
		//quite a cheeky approach to production, it constatnly tries to make out something out of the contents. if it succeeds it plays an animation and releases the product. the product was already made this whole time!
		var New;
		if(New = ComposeContents(Product,this())){
			Producing = New;
			SetAction("Production");
			return(1);
		}
	}else{
		if(!Random(2)) Smoke(0,-15,RandomX(10,20));
	}
}

func ReleaseProduct(){
	if(GetCDir() == 0 || !Producing) return(0);
	var Incr = [];
	if(GetEffect("OrderMode",this))
	  Incr = GetOrderComponentArray();
	if( (!KeepItems && GetEffect("OrderMode",this)) || !GetEffect("OrderMode",this) || (InArray(GetID(Producing),Incr) == -1 && GetEffect("OrderMode",this))){
		Producing->~Initialize();
		var ExitY = GetDefBottom()-GetY();
		ExitY-=GetDefHeight(GetID(Producing));
		Exit(Producing,20*GetCDir(),ExitY);
	}
	Producing=0;
	if(GetEffect("OrderMode",this))
		ConsumeOrder();
	return(1);
}

//for now incineration just removes object. later i will add destroyed variant.
func Incineration(){ RemoveObject(); }

func Destruction(){
	if(Producing) RemoveObject(Producing);
	if(Sign) RemoveObject(Sign);
}

public func GetResearchBase(){ return(CHEM); }

//order logic. uses an effect.
func AutoOrderComp(){ return(IsBuilt()); }

local OrderID, OrderAmounts;

func FxOrderModeStart(pTarget){
	OrderID = [];
	OrderAmounts = [];
	if(Sign) Sign->SetAction("Order");
}

func FxOrderModeStop(pTarget){
	if(Sign) Sign->SetAction("Idle");
}

func FxOrderModeTimer(pTarget,iNum,iTime){
	if(GetLength(OrderAmounts) > 0 && OrderAmounts[GetLength(OrderAmounts)-1] <= 0){
		var Index = GetLength(OrderAmounts)-1;
		ArrayDeleteIndex(OrderID,Index);
		ArrayDeleteIndex(OrderAmounts,Index);
	}

	if(GetLength(OrderID) == 0){
		Product = _MRK;
	}
	else if(Product != OrderID[GetLength(OrderID)-1]){
		Product = OrderID[GetLength(OrderID)-1];
	}
}

func OrderMenu(Caller){
	if(!GetEffect("OrderMode",this)){
		AddEffect("OrderMode",this,1,1,this);
	}

	//make menu full of current orders.
	CreateMenu(CHBS,Caller,this,0,"$TxtOrderMenu$",0,1);
	for(var i = GetLength(OrderID)-1; i >= 0; i--){
		AddMenuItem(GetName(,OrderID[i]),Format("DeleteIndex(%d,%d)",i,ObjectNumber(Caller)),OrderID[i],Caller,OrderAmounts[i]);
	}


	//Add a new order button. It begins a big journey through badly written functions to finally deliver the new order to the king of orders who makes orders. the end.
	AddMenuItem("$TxtNewOrder$","ChooseOrderItem",CHBS,Caller,0,Caller,"",2,3);
	//Button for toggling keeping stuff inside for later production.
	if(KeepItems){
	  AddMenuItem("$TxtKeep$","ToggleKeep",_MRK,Caller,0,Caller,"",2,1);
	}else{
	  AddMenuItem("$TxtKeep$","ToggleKeep",_MRK,Caller,0,Caller);
	}
}

func ToggleKeep(){
	Sound("Click");
	if(KeepItems) KeepItems = 0;
	else KeepItems = 1;
	if(Par(1))
		OrderMenu(Par(1));
}

func ChooseOrderItem(){
	var pClonk = Par(1);
	CreateMenu(GetID(),pClonk,this(),1);
	var x,i;
	while(x = GetDefinition(i++,C4D_Object)){
		if(!DefinitionCall(x,"IsChemicalProduct")) continue;
		if(!GetPlrKnowledge(GetController(pClonk),x)) continue;
		AddMenuItem("%s","SetFooItem",x,pClonk,0,pClonk);
	}
}

local fooOrder, fooNumber; //basic standstill number thingies idk :))))))))

func SetFooItem(){
	fooOrder = Par(0);
	fooNumber = 1;
	NumberMenu(0,Par(1));
}

func NumberMenu(){
	var pClonk = Par(1);
	CreateMenu(fooOrder,pClonk,this,1,"$TxtSetAmout$",0,1);
	//Amount of an item
	AddMenuItem(GetName(,fooOrder),,fooOrder,pClonk,fooNumber);
	//math
	AddMenuItem("$TxtAdd$","Addition",,pClonk,0,pClonk);
	AddMenuItem("$TxtDec$","Substraction",,pClonk,0,pClonk);
	//finish
	AddMenuItem("$TxtDone$",Format("AddOrder(%i,%d,%d)",fooOrder,fooNumber,ObjectNumber(pClonk)),_MRK,pClonk,0,0,"",2,1);
}

func Addition(){
	fooNumber++;
	NumberMenu(0,Par(1));
	Sound("Click");
}

func Substraction(){
	fooNumber--;
	if(fooNumber < 1) fooNumber = 1;
	NumberMenu(0,Par(1));
	Sound("Click");
	SelectMenuItem(2,Par(1));
}

//adds new order
public func AddOrder(pId,pAmount,Caller){
	ArrayAdd(OrderID,pId);
	ArrayAdd(OrderAmounts,pAmount);
	if(Object(Caller)){
		Sound("Ding");
		OrderMenu(Object(Caller));
	}
}

func DeleteIndex(i,Caller){
	if(i <= GetLength(OrderID)-1){
		ArrayDeleteIndex(OrderID,i);
		ArrayDeleteIndex(OrderAmounts,i);
		Sound("Click");
	}
	OrderMenu(Object(Caller));
}

func ConsumeOrder(){
	if(GetLength(OrderAmounts) > 0){
		OrderAmounts[GetLength(OrderAmounts)-1] -= 1;
	}
}


//particles :)
func DoConnectParticles(Own){
	var Amount = RandomX(8,15);
	while(Amount--) CreateParticle("MSpark",RandomX(-GetDefWidth(GetID())/2,GetDefWidth(GetID())/2),(GetDefHeight(GetID())/2)-RandomX(0,7),0,RandomX(-10,-40),RandomX(45,75),GetPlrColorDw(Own));
}

//Automatic depositing stuff
func AutoDepositHere(){ return(1); }

func AD_NeedItem(pStation){
	var x,am;
	while(am = GetComponent(0,x++,0,Product)){
		if(ContentsCount(am) < GetComponent(am,0,0,Product)) return(am);
	}
	return(0);
}