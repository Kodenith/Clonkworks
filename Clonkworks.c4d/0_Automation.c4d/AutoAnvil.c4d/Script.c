/*-- Neues Script --*/

#strict 2
#include CXEC
#include CHM2

local Product;
local Producing;

protected func Initialize(){
	Product = CNKT;
	return(1);
}

//Collection
protected func RejectCollect(idObj,pObj){
	if(GetCDir() == 0) return(1);
	
	var Prod = GetProductComponentArray();
	if(GetEffect("OrderMode",this)){
			Prod = GetOrderComponentArray();
	}
	if(InArray(idObj,Prod) != -1){
		if(pObj->~UnpackTo()){
			Enter(this,pObj);
			pObj->Unpack();
			Sound("Grapple");
			return(1);
		}
		return(0);
	}
	return(1);
}

protected func Collection(pObj,fPut){
	if(fPut) return(1);
	Sound("Grapple");
	
	var Ingr = GetProductComponentArray();
	if(GetEffect("OrderMode",this)){
			Ingr = GetOrderComponentArray();
	}
	if(InArray(GetID(pObj),Ingr) == -1){
		Exit(pObj,0,GetDefBottom()-(GetY()+5));
		return(0);
	}
	
	return(1);
}

//Setting filter. setting one automatically checks all items so they quit.
public func SetFilter(pId,pGrabber){
	if(DefinitionCall(pId,"IsAnvilProduct") != 1) return(0);
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
		if(!DefinitionCall(x,"IsAnvilProduct")) continue;
		if(!GetPlrKnowledge(GetController(pClonk),x)) continue;
		AddMenuItem("%s","SetFilter",x,pClonk,0,pClonk);
	}
}

public func ContextSetFilter(pClonk){
	[$TxtSetFilter$|Image=ANV2|Condition=IsBuilt]
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
			SetAction("Produce");
			return(1);
		}
	}
}

func TryRelease(){
	if(GetPhase() == 8){
		CastParticles("PxSpark",5,50,0,18,50,70,RGBa(255,255,0),RGBa(255,181,0));
		ReleaseProduct();
		Sound("AnvilWork*");
	}
}

func ReleaseProduct(){
	if(GetCDir() == 0 || !Producing) return(0);
	Exit(Producing,0,GetDefBottom()-(GetY()+5));
	//in case of orders, move it so it doesnt consume the item for crafting again
	if(GetEffect("OrderMode",this)){
		if(GetCDir() > 0)
			SetX(GetX(Producing)+(GetCDir()*10),Producing);
		else if(GetCDir() < 0)
			SetX(GetX(Producing)+(GetCDir()*12),Producing);
		ConsumeOrder();
	}
	Producing=0;
	return(1);
}

//for now incineration just removes object. later i will add destroyed variant.
func Incineration(){ RemoveObject(); }

func Destruction(){
	if(Producing) RemoveObject(Producing);
	if(Sign) RemoveObject(Sign);
}

public func GetResearchBase(){ return(ANVL); }

//order logic. uses an effect.
func ChooseOrderItem(){
	var pClonk = Par(1);
	CreateMenu(GetID(),pClonk,this(),1);
	var x,i;
	while(x = GetDefinition(i++,C4D_Object)){
		if(!DefinitionCall(x,"IsAnvilProduct")) continue;
		if(!GetPlrKnowledge(GetController(pClonk),x)) continue;
		AddMenuItem("%s","SetFooItem",x,pClonk,0,pClonk);
	}
}