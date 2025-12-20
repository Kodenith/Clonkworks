/*-- Neues Script --*/

#strict 2
#include LQTK
#include BAS8

func MaxAmount(){ return(18000); }
public func GetResearchBase() { return(LQTK); }
func Initialize() {
	LiquidType = 0;
	Amount = 0;
  SetAction("FillUp");
  SetPhase(0);
  return(1);
}

func Damage(){
	if(GetDamage() > 120){
		DepositLiquidPx(Amount);
		ChangeDef(DLQT,this());
		this()->Incinerate();
	}
}

func Incineration(){
	DoDamage(999);
}