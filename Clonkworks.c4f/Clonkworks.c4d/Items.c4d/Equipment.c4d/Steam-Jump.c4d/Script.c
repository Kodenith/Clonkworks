/*-- Neues Script --*/

#strict 2

local Air;

func Initialize() {
  Chargeup();
}

protected func Chargeup(){
	SetAction("Charged");
	SetPicture(0,74,64,64,this());
}

protected func DischargeAir(){
	if(GetAction() == "Charged"){
	if(!Contained()) return 0;
	
	SetAction("Charging");
	SetPicture(0,10,64,64,this());
	Sound("SteamBlast*");
	
	var vX =GetXDir(Contained());
	var vY =GetYDir(Contained());
	
	SetXDir(vX * 4, Contained());
	SetYDir(vY * 4, Contained());
	Contained()->SetAction("Tumble");
	
	for(var i = 0; i < RandomX(5,10); i++){
		CreateParticle("Steam", 0,0,RandomX(-vX,0),RandomX(-vY,0),RandomX(50,170),RGBa(255,255,255));
	}
	
	return(1);
	}
	return(0);
}

protected func ChargeAir(){
	if(GBackSky(0,0)) Air+=2;
	else Air += 1;
	
	if(Air > (38 * 120)){
		Air = 0;
		Sound("Lever1");
		Chargeup();
	}
}

public func Activate(){
	return DischargeAir();
}

public func IsAnvilProduct(){ return(true); }
public func IsAdvancedProduct(){ return(true); }