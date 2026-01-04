/*-- Neues Script --*/

#strict 2
func Hit(){
	if(!Random(3)) Sound("RockHit*");
}

func Damage(){
	if(GetDamage() > 100 && GetAction() == "Idle"){
		CastParticles("PxSpark",15,100,0,0,10,50,RGBa(255,255,0),RGBa(255,255,0));
		Sound("AnvilWork*");
		SetAction("Broken");
	}
}

func Rust(){
	SetR(0);
	SetRDir(0);
	DoCon(-1);
}

func IsAdvancedProduct(){ return(1); }
func GetResearchBase(){ return(EXDR); }