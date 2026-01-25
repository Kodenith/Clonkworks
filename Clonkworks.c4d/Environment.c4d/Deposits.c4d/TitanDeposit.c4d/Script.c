/*-- Neues Script --*/

#strict 2
#include RDEP

public func OreDepositType(){ return(ORE3); }
public func OreDepositMat(){ return("TitaniumOre"); }
func ColorMode(){ return(1); }
public func IsOreDeposit(){return(1);}

func MinOre(){ return(200); }
func MaxOre(){ return(400); }

func Coloration(){
	return(0);
}

func Damage(iChange){
	if(GetDamage() >80 &&!OnFire() && !FindObject(TITM) ){
		SetSolidMask();
		var targ = RandomX(3,8);
		for(var i = 0; i < targ; i++){
				Fling(CreateObject(OreDepositType()),RandomX(-6,6),RandomX(-6,6));
		}
		Explode(28);
		return(1);
	}
}