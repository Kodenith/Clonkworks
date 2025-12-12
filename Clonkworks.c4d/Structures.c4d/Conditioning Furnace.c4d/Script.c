/*-- Neues Script --*/

#strict 2
#include BAS4
#include CXEC

local iFuel;
local Delay;

public func TempRange(){ return(30); }

func Initialize() {
  iFuel = 0;
  return(1);
}

func Check(){
if(Delay){
	Delay--;
	return(0);
}
	if(!EnergyCheck(30)){
		iFuel = 0;
		Delay = 38;
		return(0);
	}
	
	//idle only stuff
	if(GetAction() != "Idle") return(0);
	var find;
	if(find = FindContents(COAL)){
		if(!Random(3)) ChangeDef(ASHS,find);
		else RemoveObject(find);
		
		iFuel = 38*15;
		SetAction("Heating");
		Sound("Fuse");
		return(1);
	}
	
	if(find = FindContents(CRYS)){
		RemoveObject(find);
		
		iFuel = 38*15;
		SetAction("Cooling");
		Sound("Fuse");
		return(1);
	}
}

//heating and cooling effects
public func DoHeat(){
	if(InLiquid() || !iFuel){
		Sound("Discharge");
		SetAction("Idle");
		return(0);
	}
	
	iFuel--;
	if(!iFuel){
		var find;
		if(find = FindContents(COAL)){
		if(!Random(3)) ChangeDef(ASHS,find);
		else RemoveObject(find);
		
		iFuel = 38*15;
		return(1);
		}
	}
	
	CreateParticle("PSpark",RandomX(-20,20),RandomX(-10,20),0,0,RandomX(25,40),RGBa(255,140,0),this());
	if(!Random(5)) Smoke(GetVertex(6,0),GetVertex(6,1),RandomX(5,15));
	//finding a random pixel nearby and heating it up!
	for(var i = 0; i < 600; i++){
	var iX, iY, range;
	range = 400;
	iX = GetX() + RandomX(-range,range);
	iY = GetY() + RandomX(-range,range);
	if(Distance(GetX(),GetY(),iX,iY) > 400) continue;
	
	var mat = GetMaterial(iX-GetX(),iY-GetY());
	var HeatedMat, MatTex;
	HeatedMat = GetMaterialVal("AboveTempConvertTo", "Material", mat);
	if(GetMaterialVal("AboveTempConvert", "Material", mat) > TempRange()) continue;
	if(!HeatedMat || HeatedMat == "") continue;
	MatTex = GetMaterialVal("TextureOverlay", "Material", Material(HeatedMat));
	
	var Underground = UndergroundXY(iX,iY);
	if(MatTex){
		CreateMatPx(Format("%s-%s",HeatedMat,MatTex),iX,iY,Underground);
	}else{
		CreateMatPx(Format("%s",HeatedMat),iX,iY,Underground);
	}
	}
}

public func DoCool(){
	if(InLiquid() || !iFuel){
		Sound("Discharge");
		SetAction("Idle");
		return(0);
	}
	
	iFuel--;
	if(!iFuel){
		var find;
		if(find = FindContents(CRYS)){
		RemoveObject(find);
		
		iFuel = 38*15;
		return(1);
		}
	}
	
	CreateParticle("PSpark",RandomX(-20,20),RandomX(-10,20),0,0,RandomX(25,40),RGBa(0,140,255),this());
	//finding a random pixel nearby and cooling it off!
	for(var i = 0; i < 600; i++){
	var iX, iY, range;
	range = 400;
	iX = GetX() + RandomX(-range,range);
	iY = GetY() + RandomX(-range,range);
	if(Distance(GetX(),GetY(),iX,iY) > 400) continue;
	
	
	
	var mat = GetMaterial(iX-GetX(),iY-GetY());
	var HeatedMat, MatTex;
	HeatedMat = GetMaterialVal("BelowTempConvertTo", "Material", mat);
	if(GetMaterialVal("BelowTempConvert", "Material", mat) < -TempRange()) continue;
	if(!HeatedMat || HeatedMat == "") continue;
	MatTex = GetMaterialVal("TextureOverlay", "Material", Material(HeatedMat));
	
	var Underground = UndergroundXY(iX-GetX(),iY-GetY());
	
	if(MatTex){
		CreateMatPx(Format("%s-%s",HeatedMat,MatTex),iX,iY,Underground);
	}else{
		CreateMatPx(Format("%s",HeatedMat),iX,iY,Underground);
	}
	}
}

func Damage(){
	if(GetDamage() > 120){
		if(GetAction() != "Idle"){
			for(var i = 6; i > 0; i--){
				var flame = CreateObject(FLAM);
				Fling(flame,RandomX(-5,5),RandomX(-5,5));
			}
		}
		Explode(40);
	}
}

//context
func ContextHeat(pClonk){
	[$TxtFuel1$|Image=COAL]
	var NearbyCoal = FindObjects(Find_ID(COAL),Find_Distance(100));
	var Len = Min(GetLength(NearbyCoal),20);
	for(var i = 0; i < Len; i++){
		AddCommand(pClonk,"Put",this(),0,0,NearbyCoal[i]);
	}
}

func ContextCool(pClonk){
	[$TxtFuel2$|Image=CRYS]
	var NearbyCoal = FindObjects(Find_ID(CRYS),Find_Distance(100));
	var Len = Min(GetLength(NearbyCoal),20);
	for(var i = 0; i < Len; i++){
		AddCommand(pClonk,"Put",this(),0,0,NearbyCoal[i]);
	}
}