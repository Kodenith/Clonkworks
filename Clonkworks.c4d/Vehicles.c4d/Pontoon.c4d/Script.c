/*-- Neues Script --*/

#strict 2
local iFuel;

func Initialize() {
  SetAction("OnGround");
  return(1);
}

func ControlThrow(pClonk){
	[$TxtRefuel$]
	if(iFuel < 600){
		if(GetID(pClonk->Contents(0)) == OBRL){
			var Barl = pClonk->Contents(0);
			iFuel += Barl->GetAmount();
			ChangeDef(BARL,Barl);
			Sound("Splash1");
		}
	}
}
func Incineration(){
	Explode(45);
}

func ControlDigDouble(){
	[$TxtCheck$]
	Message("$TxtFuel$",this(),iFuel);
	Sound("Ding");
}

func Change(){
	var dir = GetDir();
	ChangeDef(PNT2,this());
	this()->SetDir(dir);
	this()->SetAction("NoFuel");
}

func Unidle(){
	if(FindObject(FUDS) || iFuel > 600) iFuel = 600;
	if(GetAction()=="Idle") SetAction("OnGround");
}

//fuel Station Logic
public func REFUNeedFuel(){
	if(iFuel < 600) return(1);
}
public func REFUFuelType(){
	return([OBRL]);
}

public func HowToREFU(pFuel){
			var Barl = pFuel;
			iFuel += Barl->GetAmount();
			ChangeDef(BARL,Barl);
	return(1);
}