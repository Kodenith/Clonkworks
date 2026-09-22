/*-- Neues Script --*/

#strict 2
#include RDEP

public func OreDepositType(){ return(CRYS); }
public func OreDepositMat(){ return("Crystal"); }
func ColorMode(){ return(0); }
public func IsOreDeposit(){return(1);}

func MinOre(){ return(50); }
func MaxOre(){ return(100); }

func Coloration(){
	return(0);
}

func Damage(iChange){
	if(GetDamage() > 80 &&!OnFire() && !FindObject(OREM) ){
		SetSolidMask();
		var targ = RandomX(3,8);
		for(var i = 0; i < targ; i++){
				Fling(CreateObject(OreDepositType()),RandomX(-6,6),RandomX(-6,6));
		}
		Explode(28);
		return(1);
	}
}

func Initialize() {
  _inherited();
  var phase = 0;
  SetPhase(phase = Random(3));
  SetSolidMask(phase*48, 0, 48, 48,0,0,this());
  SetPicture(phase*48, 0, 48, 48,this());
  return(1);
}