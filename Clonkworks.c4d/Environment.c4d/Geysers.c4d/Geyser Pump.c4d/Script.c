/*-- Geyser Pump --*/

#strict 2
#include BAS3
#include IO__

local Extract; //Either the string name of the material, or the id of the gas.
local Amount; //Amount of material or gas. if it runs out it stops extracting.

//wiring

public func InputList(){
  return(["Lock"]);
}

//this function returns a list of string that can be output.
//default is a single output.
public func OutputList(){
  return(["Extracted Material/Gas","Material/Gas Left"]);
}

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
    if(OutputName == "Extracted Material/Gas"){
      return(Extract);
    }

    if(OutputName == "Material/Gas Left"){
      return(Amount);
    }
}

//custom placement requirements
public func RejectConstruction(iX,iY,pBuilder){
	var LiquidGeyser = FindObject2(Find_AtPoint(GetX(pBuilder),GetY(pBuilder)),Find_ID(GEY1));
  var GasGeyser = 0; //TODO: Add this

  if(!LiquidGeyser && !GasGeyser){
    Message("<c ff0000>$Warning1$</c>",pBuilder);
    pBuilder->Sound("Error");
    return(1);
  }
	
	return(0);
}

protected func Initialize(){
  var LiquidGeyser = FindObject2(Find_AtPoint(0,10),Find_ID(GEY1));
  var GasGeyser = 0; //TODO: Add this

  if(LiquidGeyser){
    Extract = LocalN("Liquid",LiquidGeyser);
    Amount = 20000;  //may be either low or high, can be changed later if so.
    RemoveObject(LiquidGeyser);
  }else if(GasGeyser){
    //TODO: Add this
  }else{
    Extract = 0;
    Amount = 0; //no data, machine does not function.
  }
}

protected func Manage(){
  //Manage action based on energy and data.
  var pDrainPipe = FindObject(DPIP,0,0,0,0,0,"Connect",this());
  var CanPump = (Extract != 0 && Amount > 0 && EnergyCheck(10000) && GetCon() >= 100 && pDrainPipe && !InputActive("Lock"));
  if(CanPump){
    if(GetAction() == "Idle") SetAction("Pumping");
  }else{
    if(GetAction() != "Idle") SetAction("Idle");
  }
}

protected func PumpOut(){
  //pump out either liquid or gas. both are their own functions.
  if(GetType(Extract) == C4V_String){
    PumpLiquid();
  }
}

//Liquid Pumping
protected func PumpLiquid(){
  var pDrainPipe = FindObject(DPIP,0,0,0,0,0,"Connect",this());
  var pPumpTarget = GetActionTarget(1, pDrainPipe);

  if(!pDrainPipe) return(0);

  //Line kits simply drop pixels around.
  if(GetID(pPumpTarget) == LNKT){
    pPumpTarget->CastPXS(Extract,1,1);
    Amount--;
    return(0);
  }

  //containers have their barrels filled.
  if(GetOCF(pPumpTarget) & OCF_Container){
      //check for barrels that aren't empty.
      var Set = 0;
      var BarrelType = GetBarrelType(Material(Extract));
      if(BarrelType){
        for(var i in FindObjects(Find_ID(BarrelType),Find_Container(pPumpTarget))){
          if(!i->BarrelIsFull()){
            i->BarrelDoFill(1,Material(Extract));
            Amount--;
            Set=1;
            break;
          }

        }

      }else{  
        return(0);
      }

      if(Set) return(0);

      //otherwise fill an empty barrel
      var Barrel = FindContents(BARL,pPumpTarget);
      if(Barrel){
        ChangeDef(BarrelType,Barrel);
        Barrel->BarrelDoFill(1,Material(Extract));
      }
  }
}

public func GetResearchBase() { return(PUMP); }

func Damage(){
  if(GetDamage() > 100) Incinerate();
}

func Incineration(){
  //explode everything inside before burning.
  if(GetType(Extract) == C4V_String){
    CastPXS(Extract,Max(Amount,1000),100,0,-10);
  }

  ChangeDef(DGPP);
}