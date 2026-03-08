/* INPUT OUTPUT BY KODENITH */
//wire color is 343
#strict 2

//can this be wired from and to?
public func WireFrom(){ return(GetCon() > 99); }
public func WireTo(){ return(GetCon() > 99); }

//is this a circuit?
public func IsCircuit(){ return(1); }

//this function returns a list of string that can be input.
//default is two inputs
public func InputList(){
  return(["A","B"]);
}

//this function returns a list of string that can be output.
//default is a single output.
public func OutputList(){
  return(["Out"]);
}

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
    return(1);
}

//check if the wire is ticking a certain input in this.
public func InputActive(string InputName){
  for(var Wire in FindObjects(Find_ID(WIRE))){
      if(GetActionTarget(1,Wire) == this){
          if(Wire->GetOutputValue() && Wire->ConnectedTo() == InputName) return(Wire->GetOutputValue());
      }
  }
}

//does an input have a wire connected to it?
public func DoesInputHaveWire(string InputName){
  for(var Wire in FindObjects(Find_ID(WIRE))){
      if(GetActionTarget(1,Wire) == this){
         if(Wire->ConnectedTo() == InputName) return(1);
      }
  }
}

/* WIRE CLEAN MENU */

public func ControlDigDouble(pClonk){
  [$TxtRemoveContext$|Image=IO__:1|Conditon=WireFrom]
  if(GetAction(pClonk) != "Push" ) return(0);
  if(GetActionTarget(0,pClonk) != this) return(0);

  if(!WireFrom()){
    Message("$TxtCantRO$",pClonk);
    return(0);
  }

  CreateMenu(GetID(),pClonk,this,0,"$TxtRemoveMenu$",0,1);
  for(var Output in FindObjects(Find_ID(WIRE),Find_ActionTarget(this))){
    if(GetID(GetActionTarget(1,Output)) == WRKT) continue;
    AddMenuItem(Format("$TxtOutputOption$",LocalN("OutputString",Output),GetName(GetActionTarget(1,Output)),LocalN("InputString",Output)),Format("DisconnectWire(Object(%d),Object(%d))",ObjectNumber(Output),ObjectNumber(pClonk)),WIRE,pClonk);
  }
}

public func DisconnectWire(Wire,Clonk){
  if(Clonk){
      if(GetAction(Clonk) != "Push" ) return(0);
      if(GetActionTarget(0,Clonk) != this) return(0);
  }

  if(!GetActionTarget(1,Wire)) return(0);

  GetActionTarget(1,Wire)->Sound("Click");
  GetActionTarget(1,Wire)->CastObjects(WRKT,1,10);
  RemoveObject(Wire);
  Sound("Click");
  if(Clonk) ControlDigDouble(Clonk);
}

public func ContextClearWires(pClonk){
  [$TxtRemoveContext$|Image=IO__:1|Conditon=WireFrom]
  AddCommand(pClonk,"Call",this,pClonk,0,0,0,"ControlDigDouble");
  AddCommand(pClonk,"Grab",this);
}

/* COLORATION
only for context menu */

public func AllowRecolor(){ return(GetCon() > 99 && GetDefCoreVal("ColorByOwner","DefCore",GetID()) == 1); }

public func ContextRecolor(pClonk){
  [$TxtColorContext$|Image=WATR|Conditon=AllowRecolor]
  if(!AllowRecolor()){
    Message("$TxtCantRecolor$",pClonk);
    return(0);
  }

  if(GetAction(pClonk) != "Push" && GetActionTarget(0,pClonk) != this){
    AddCommand(pClonk,"Call",this,pClonk,0,0,0,"ContextRecolor");
    AddCommand(pClonk,"Grab",this);
    return(0);
  }

  CreateMenu(WATR,pClonk,this,0);
  var Colors = [RGBa(255,0,0),RGBa(255,255/2,0),RGBa(255,255,0),RGBa(0,255,0),RGBa(0,255,255),RGBa(0,255/2,255),RGBa(0,0,255),RGBa(170,0,255),RGBa(255,0,255),RGBa(255,0,138),RGBa(255,255,255)];
  for(var c in Colors){
    AddMenuItem("$TxtColorSelect1$",Format("TryRecolor(%d,Object(%d))",c,ObjectNumber(pClonk)),IO__,pClonk,0,," ",7,2,c);
  }

  for(var i = 0; i < GetPlayerCount(); i++){
    var c = GetPlrColorDw(GetPlayerByIndex(i));
    AddMenuItem(Format("$TxtColorSelect2$",GetPlayerName(GetPlayerByIndex(i))),Format("TryRecolor(%d,Object(%d))",c,ObjectNumber(pClonk)),FLAG,pClonk,0,," ",6,c);
  }
}

func TryRecolor(Color,Clonk){
  if(GetAction(Clonk) != "Push" ) return(0);
  if(GetActionTarget(0,Clonk) != this) return(0);

  SetColorDw(Color);
  Message("$TxtColorSet$",this);
  Sound("Splash1");
}