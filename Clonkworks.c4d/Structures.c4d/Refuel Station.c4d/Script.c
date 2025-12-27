/*-- Neues Script --*/

#strict 2
#include BAS8
#include CXEC

func Initialize() {  
  return(1);
}

private func XFuel(){ return(GetVertex(0,0)); }
private func XLeft(){ return(GetVertex(4,0)); }
private func XRigt(){ return(GetVertex(5,0)); }


//entrance
func RejectCollect(pObj){
	if(pObj->~IsREFUItem()) if(ContentsCount() < 25) return(0);
	return(1);
}

//grabbing
func RejectGrabbed(pObj){
	var iX = AbsX(GetX(pObj));
	if(!Inside(iX,XLeft(),XRigt())) return(1);
}

func Grabbed(pObj,fGrab){
	if(FindObject2(Find_Action("Push"),Find_ActionTarget(this()),Find_Exclude(pObj))) return(1);
	if(fGrab && GetAction() == "Idle"){
		SetAction("Open");
	}
	
	if(!fGrab && GetAction() == "Opened"){
		SetAction("Close");
	}else if(!fGrab && GetAction() == "Open"){
		var Ph = GetPhase();
		SetAction("Close");
		SetPhase(7-Ph);
	}
	
	return(1);
}

func SoundOpen(){
	return(Sound("Airlock1"));
}

func SoundClose(){
	return(Sound("Airlock2"));
}

func CheckGrabbed(){
	if(!FindObject2(Find_Action("Push"),Find_ActionTarget(this()))) SetAction("Close");
}

//checking and refuel
func DoChecks(){
	if(!EnergyCheck(300)){
		if(GetAction() == "Refuel") SetAction("RefuelStop");
		return(0);
	}
	if(GetAction() != "Idle") return(0);
	
	if(FindRefuelable() && Abs(GetXDir(FindRefuelable())) < 10){
		for(var i in FindRefuelable()->REFUFuelType()){
			if(FindContents(i)){
			SetAction("RefuelStart");
			return(1);
			}
		}
	}
}

func CheckRefuel(){
	var FuelTo = FindRefuelable();
	if(!FuelTo){
		SetAction("RefuelStop");
		return(0);
	}
	
	var UseItem;
	for(var i in FindRefuelable()->REFUFuelType()){
		if(FindContents(i)){
			UseItem = FindContents(i);
			break;
		}
	}
	
	if(!UseItem){
		SetAction("RefuelStop");
		return(0);
	}
	
	FuelTo->~HowToREFU(UseItem);
	Sound("JarFIll");
}

func FindRefuelable(){
	var ref = FindObject2(Find_Func("REFUNeedFuel"),Find_AtPoint(XFuel(),0));
	if(ref) return(ref);
}

//Destruction
func Damage(){
	if(GetDamage() > 50){
		for(var i = 0; i < 8; i++){
			var flame = CreateObject(FLAM,RandomX(-5,5),RandomX(-5,5)-10);
			Fling(flame,RandomX(-10,10),RandomX(-10,10));
		}
		Explode(50);
	}
}