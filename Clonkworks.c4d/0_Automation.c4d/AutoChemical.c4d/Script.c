/*-- Neues Script --*/

#strict 2
#include CXEC

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

//Collection, based on conveyor direction. i cant have both pipes on the sprite go to waste :)
protected func RejectCollect(idObj,pObj){
	if(GetCDir() == 0) return(1);
	if(pObj->~UnpackTo()){
		pObj->Unpack();
		return(1);
	}
	if(GetCDir() == 1){
		if(GetX(pObj) < GetX()-15) return(0);
	}
	if(GetCDir() == -1){
		if(GetX(pObj) > GetX()+15) return(0);
	}
	
	return(1);
}

protected func Collection(pObj,fPut){
	if(fPut) return(1);
	Sound("Clonk");
	
	var Ingr = GetProductComponentArray();
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
	Exit(Producing,20*GetCDir(),GetDefBottom()-GetY());
	Producing=0;
	return(1);
}

//for now incineration just removes object. later i will add destroyed variant.
func Incineration(){ RemoveObject(); }

func Destruction(){
	if(Producing) RemoveObject(Producing);
	if(Sign) RemoveObject(Sign);
}

public func GetResearchBase(){ return(CHEM); }