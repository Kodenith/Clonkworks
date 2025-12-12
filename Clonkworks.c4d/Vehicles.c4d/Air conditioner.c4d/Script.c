/*-- Neues Script --*/

#strict 2

local iColdFuel, iHotFuel;

func Initialize() {
	iColdFuel = 0;
	iHotFuel = 0;
  SetAction("Idle");
  return(1);
}

func DoWarn(cont){
	Sound("Error");
	if(!FindObject2(Find_ID(WR_F), Find_ActionTarget(this()))){
			FuelWarn(this(),[COAL,CRYS],cont+1);
	}
	return(1);
}

public func ControlUpDouble(pByObj){
	[$TxtLeft$]
	if(iHotFuel < 0) iHotFuel = 0;
	if(iColdFuel < 0) iColdFuel = 0;
	
	if(GetAction() == "Heating") return(1);
	if(GetAction() == "Cooling"){
		Sound("Click");
		return(SetAction("Idle"));
	}
	if(!FuelCheck(COAL) && !FindObject(FUDS)) return(DoWarn(GetController(pByObj)));
	if(GetAction() == "Idle"){
		Sound("Click");
		return(SetAction("Heating"));
	}

}

public func ControlDigDouble(pByObj){
	[$TxtRight$]
	if(iHotFuel < 0) iHotFuel = 0;
	if(iColdFuel < 0) iColdFuel = 0;
	
	if(GetAction() == "Cooling") return(1);
	if(GetAction() == "Heating"){
		Sound("Click");
		return(SetAction("Idle"));
	}
	if(!FuelCheck(CRYS) && !FindObject(FUDS)) return(DoWarn(GetController(pByObj)));
	if(GetAction() == "Idle"){
		Sound("Click");
		return(SetAction("Cooling"));
	}
}

public func FuelCheck(type){
	if(FindObject(FUDS)) return(1);
	if(iHotFuel < 1 && type == COAL){
		if(FindContents(COAL)){
			RemoveObject(FindContents(COAL));
			iHotFuel+=(36*5);
		}
		
		if(iHotFuel < 1) return(0);
	}
	
	if(iColdFuel < 1 && type == CRYS){
		if(FindContents(CRYS)){
			RemoveObject(FindContents(CRYS));
			iColdFuel+=(36*5);
		}
		
		if(iColdFuel < 1) return(0);
	}
	
	return(1);
}

public func RejectCollect(thing){
	if(thing == COAL || thing == CRYS) return(0);
	return(1);
}

public func CanNotBeDispensedInto(item){
	return(RejectCollect(GetID(item)));
}

//heating and cooling effects
public func DoHeat(){
	if(!FuelCheck(COAL)){
		Sound("Discharge");
		SetAction("Idle");
		return(0);
	}
	
	if(InLiquid()){
		Sound("Discharge");
		SetAction("Idle");
		return(0);
	}
	
	iHotFuel--;
	
	CreateParticle("PSpark",RandomX(-8,8),RandomX(-5,5),0,0,RandomX(25,40),RGBa(255,140,0),this());
	if(!Random(5)) Smoke(0,-5,RandomX(5,15));
	//finding a random pixel nearby and heating it up!
	for(var i = 0; i < 70; i++){
	var iX, iY, range;
	range = 75;
	if(Contained()) range/=2;
	iX = GetX() + RandomX(-range,range);
	iY = GetY() + RandomX(-range,range);
	if(Distance(GetX(),GetY(),iX,iY) > range) continue;
	
	//if connected to something similiar to a train, do heating there.
	if(GetID(Contained()) == WAGN){
		var Train = FindObject2(Find_Func("IsTrainLike"), Find_Action("Push"), Find_ActionTarget(Contained()));
		if(Train){
			iX += AbsX(GetX(Train));
			iY += AbsY(GetY(Train));
		}
	}
	
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

public func DoCooling(){
	if(!FuelCheck(CRYS)){
		Sound("Discharge");
		SetAction("Idle");
		return(0);
	}
	
	if(InLiquid()){
		Sound("Discharge");
		SetAction("Idle");
		return(0);
	}
	
	iColdFuel--;
	
	CreateParticle("PSpark",RandomX(-8,8),RandomX(-5,5),0,0,RandomX(25,40),RGBa(0,140,255),this());
	//finding a random pixel nearby and cooling it off!
	for(var i = 0; i < 70; i++){
	var iX, iY, range;
	range = 75;
	if(Contained()) range/=2;
	iX = GetX() + RandomX(-range,range);
	iY = GetY() + RandomX(-range,range);
	if(Distance(GetX(),GetY(),iX,iY) > range) continue;
	
	//if connected to something similiar to a train, do heating there.
	if(GetID(Contained()) == WAGN){
		var Train = FindObject2(Find_Func("IsTrainLike"), Find_Action("Push"), Find_ActionTarget(Contained()));
		if(Train){
			iX += AbsX(GetX(Train));
			iY += AbsY(GetY(Train));
		}
	}
	
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

public func TempRange(){ return(15); }

//basic stuff
public func IsAdvancedProduct(){ return(1); }



//global funcs
global func UndergroundXY(int x, int y){
	var iX = AbsX(x);
	var iY = AbsY(y);
	var iMat = GetMaterial(iX,iY);
	var iTex = GetMaterialVal("TextureOverlay", "Material", iMat);
	//remove pixel here 20 times just in case.
	for(var i = 0; i < 20; i++){
	BlastFree(iX,iY,1);
	}
	
	if(GetMaterial(iX,iY) != Material("Sky")){
		if(iTex){
			CreateMatPx(Format("%s-%s",MaterialName(iMat),iTex),x,y,1);
		}else{
			CreateMatPx(Format("%s",MaterialName(iMat)),x,y,1);
		}
		return(1);
	}
	
	if(iTex){
			CreateMatPx(Format("%s-%s",MaterialName(iMat),iTex),x,y,0);
	}else{
			CreateMatPx(Format("%s",MaterialName(iMat)),x,y,0);
	}
	return(0);
}

//damage
public func Damage(int iChange, int iByPlayer){
	if(GetDamage() > 55) Explode(30);
}

public func ALKConnectType(){
	return([FNPP]);
}

//fuel Station Logic
public func REFUNeedFuel(){
	if(FindObject(FUDS)) return(0);
	if(ContentsCount() > 24) return(0);
	if(ContentsCount(COAL) < 25 || ContentsCount(CRYS) < 25) return(1);
}
public func REFUFuelType(){
	return([COAL,CRYS]);
}

public func HowToREFU(pFuel){
	Enter(this(),pFuel);
	return(1);
}

public func GetResearchBase(){ return(CFUR); }