#strict 2
#include S_LQ

func Initialize() {
	LiquidType = 0;
	Amount = 0;
  SetAction("FillUp");
  SetPhase(0);
  return(_inherited());
}

func MaxAmount(){ return(6500); }
func AmountDevision(){ return(MaxAmount()/20);}

func Hit3(){
	OutsertLiquidPx(Amount);
	Sound("ClonkHit*");
}

func LiquidCheck(){
	SetPhase(Min(Amount / AmountDevision(),19));
	
	if(LiquidType && Amount != 0){
	var r, g, b;
	r = GetMaterialVal("Color", "Material", Material(LiquidType),0);
	g = GetMaterialVal("Color", "Material", Material(LiquidType),1);
	b = GetMaterialVal("Color", "Material", Material(LiquidType),2);
	SetColorDw(RGBa(r,g,b), this());
	}
}

public func IsAdvancedProduct(){ return(1); }

public func ControlThrow(pByObj){
	[$Fill$|Image=L_FL]
	
	//controller must be holding this
	if(pByObj->GetAction() != "Push") return(0);
	if(GetActionTarget(0,pByObj) != this()) return(0);
	
	//check if player is holding barrel
	var Count = ContentsCount(,pByObj);
	var barrel;
	for(var i = 0; i < Count; i++){
		if(Contents(i,pByObj)->~BarrelMaxFill()){
			barrel = Contents(i,pByObj);
			break;
		}
		
		if(GetID(Contents(i,pByObj)) == CNCR){
			InsertLiquidPx("Rock",100);
			Sound("Splash1");
			RemoveObject(Contents(i,pByObj));
			return(1);
		}
		
		if(GetID(Contents(i,pByObj)) == LIQG){
			InsertLiquidPx("Granite",150);
			Sound("Splash1");
			RemoveObject(Contents(i,pByObj));
			return(1);
		}
	}
	
	if(GetID(barrel) == BARL) {
		//check if has enough liquid
		if(Amount < barrel->BarrelMaxFill()){
			Message("$FillEmpty$",this(),LiquidType);
			Sound("CommandFailure1");
			return(1);
		}
		//empty? fill it up!
		if(Amount >= barrel->BarrelMaxFill()){
			var LQT = GetBarrelType(Material(LiquidType));
			if(LQT){
				ChangeDef(LQT,barrel);
			}
			else{
				Message("$FillFail$",this(),LiquidType);
				Sound("CommandFailure1");
				return(1);
			}
			
			LocalN("iFillLevel", barrel) = barrel->BarrelMaxFill();
			Amount -= barrel->BarrelMaxFill();
			Sound("Splash2");
		}
		
		return(1);
	}
	
	//dispense liquid
	if(!barrel){
		Message("$FillNoBarrel$",this(),LiquidType);
		Sound("CommandFailure1");
		return(1);
	}
	
	var mat, cnt;
	cnt = barrel->GetAmount();
	mat = barrel->BarrelMaterialName();
	if(!cnt || !mat) return(1);
	
	barrel->BarrelDoFill(-cnt);
	ChangeDef(BARL, barrel);
	InsertLiquidPx(mat, cnt);
	Sound("Splash1");
	
	return(1);
}

public func ContextFill(pByObj){
	[$Fill$|Image=L_FL]
	SetCommand(pByObj, "Grab", this());
	AppendCommand(pByObj, "Call", this(), pByObj, 0, 0,,"ControlThrow");
}

public func ControlDig(pByObj){
	[$Release$|Image=L_RL]
	if(Amount != 0){
		Sound("AirLock2");
		OutsertLiquidPx(Amount);
	}
}

public func ContextRelease(pByObj){
	[$Release$|Image=L_RL|Condition=HasLiquid]
	SetCommand(pByObj, "Grab", this());
	AppendCommand(pByObj, "Call", this(), pByObj, 0, 0,,"ControlDig");
}

private func MaxContents() { return(0); }
func RejectContents(){ return(1); }
func HasLiquid(){ return(Amount > 0); }
func IsFull(){ return(Amount >= MaxAmount()); }

public func ALKConnectType(){
	//only 1 hose can be connected to a tank.
	if(FindObject2(Find_ID(HOSE),Find_ActionTarget(this()))) return(0);
	return([HOSE]);
}

// leftover from wagon
protected func ControlLeft(object clonk)
{
  [$TxtLeft$|Image=STMS:1]
  var pEngine = FindObject(0,0,0,0,0,OCF_Grab,"Push",this());
  if(!pEngine) return(0);
  pEngine->~ControlLeft(clonk);
  return(1);
}

protected func ControlRight(object clonk)
{
  var pEngine = FindObject(0,0,0,0,0,OCF_Grab,"Push",this());
  if(!pEngine) return(0);
  pEngine->~ControlRight(clonk);
  return(1);
}

protected func ControlLeftDouble(object clonk)
{
  var pEngine = FindObject(0, 0,0,0,0, OCF_Grab, "Push",this());
  if(!pEngine) return(0);
  pEngine->~ControlLeftDouble(clonk);
  return(1);
}

protected func ControlRightDouble(object clonk)
{
  var pEngine = FindObject(0, 0,0,0,0, OCF_Grab, "Push",this());
  if(!pEngine) return(0);
  pEngine->~ControlRightDouble(clonk);
  return(1);
}

protected func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
  var pEngine = FindObject(0, 0,0,0,0, OCF_Grab, "Push",this());
  if(pEngine) pEngine->~ControlUpdate(clonk, comdir, dig, throw);
}

protected func ControlDownSingle(object clonk)
{
  var pEngine = FindObject(0, 0,0,0,0, OCF_Grab, "Push",this());
  if(!pEngine) return(0);
  pEngine->~ControlDownSingle(clonk);
  return(1);
}
public func GetResearchBase() { return(PUMP); }

//Refuel
public func REFUNeedFuel(){
	if(Amount)
		if(!GetBarrelType(Material(LiquidType))) return(0);
	if(Amount < MaxAmount()) return(1);
}
public func REFUFuelType(){
	var LQT = GetBarrelType(Material(LiquidType));
	if(LQT) return([LQT]);
	if(Amount == 0) return(BarrelList());
}

public func HowToREFU(pFuel){
	var lq = pFuel->~BarrelMaterialName();
	var cnt = pFuel->GetAmount();
	pFuel->BarrelDoFill(-cnt);
	ChangeDef(BARL,pFuel);
	InsertLiquidPx(lq,cnt);
}

//INFOBAR
public func InfobarTrigger(){
	return(IB_Grab);
}

public func InfobarMax(){
	return(MaxAmount());
}

public func InfobarValue(){
	return(Amount);
}

public func InfobarColor(){
	var color = RGBa(0,0,1);
	if(LiquidType) color = GetColorDw();
	return(color);
}

public func InfobarInfo(){
	if(Amount) return(LiquidType);
	return("$FxEmpty$");
}