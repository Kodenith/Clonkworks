/*-- Neues Script --*/

#strict 2
#include RDEP

public func OreDepositType(){ return(ORE1); }
public func OreDepositMat(){ return("Ore"); }
func ColorMode(){ return(1); }

func MinOre(){ return(14); }
func MaxOre(){ return(45); }

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