/*-- Leitungsbausatz --*/

#strict


protected func Hit()
{
  Sound("RockHit*");
}

protected func Activate(clonk)
{
  [$TxtConnectline$]
  var obj = FindObject2(Find_OCF(OCF_LineConstruct()),Find_OnLine(0,0,0,18),Find_NoContainer());
  if(!obj) return(Message("$TxtNoNewLine$", this()) );
  var pLine = FindObject(0, 0,0,0,0, 0, "Connect", this() );
  SetComDir(COMD_Stop(), clonk);

  if(pLine)
  {
    if(obj == GetActionTarget(0, pLine) || obj == GetActionTarget(1, pLine) )
    {
      // Leitung wieder entfernen
      Message("$TxtLineRemoval$", this(), GetName(pLine) );
      RemoveObject(pLine);
      Sound("Connect");
      return(1);
    }
    else
    {
      ConnectLine(pLine, obj);
    }
  }
  else
  {
    LineSelection(obj, clonk);
  }

  return(1);
}

func LineSelection(obj,clonk){
  if(obj->~WireFrom()){
      CreateMenu(IO__,clonk,this,0,"$TxtOutput$",0,1);
      for(var i in obj->~OutputList()){
        AddMenuItem(i,Format("CreateLine(WIRE,%d,Object(%d),Object(%d),\"%s\")",GetOwner(clonk),ObjectNumber(obj),ObjectNumber(this),i),WIRE,clonk);
      }
  }else{
      Sound("Error");
      Message("$TxtNoConnectType$",this(),GetName(,WIRE),GetName(obj));
  }
}

private func CreateLine(linetype, owner, from, to,output)
{
  if(InArray(from,FindObjects(Find_AtPoint())) == -1) return(0);
  var line = CreateObject(linetype, 0, 0, owner);
  LocalN("OutputString",line) = output;
  ObjectSetAction(line, "Connect", from, to);
  Message("$TxtNewLine$", this(), GetName(line) , output);
  Sound("Connect");
  return(line);
}

/* Verbinden zweier Geb�ude mit einer Leitung */
private func ConnectLine(line, to)
{
  var line_accept;
  if(to->~WireTo() && GetActionTarget(1,line) == this()){
		  line_accept = true;
  }
  else{
	  line_accept = false;
  }

  var t1 = GetActionTarget(0, line);
  var t2 = to;
  if(t1 == this() ) { t2 = GetActionTarget(1, line); t1 = to; }
  
  var ok = false;
    if (t1 == to && line_accept)
      ok = true;
    if (t2 == to && line_accept)
      ok = true;


  if(!ok)
  {
    Sound("Error");
    Message("$TxtNoConnectType$", this(), GetName(line), GetName(to) );
    return(0);
  }

  /*
  ObjectSetAction(line, "Connect", t1, t2);

  Message("$TxtConnect$", Contained(), GetName(line), GetName(to) );
  Sound("Connect");
  RemoveObject();
  */
    
  CreateMenu(IO__,Contained(),this,0,"$TxtInput$",0,1);
  for(var i in to->~InputList()){
        //only one output can be connected to an input
       if(to->DoesInputHaveWire(i) && !FindObject(MLIO)) continue;
       AddMenuItem(i,Format("FinishConnection(Object(%d),Object(%d),\"%s\")",ObjectNumber(to),ObjectNumber(line),i),WIRE,Contained());
  }

  return(1);
}

func FinishConnection(to,line,input){
  if(InArray(to,FindObjects(Find_AtPoint())) == -1) return(0);
  var t1 = GetActionTarget(0, line);
  var t2 = to;
  ObjectSetAction(line, "Connect", t1, t2);
  LocalN("InputString",line) = input;
  Message("$TxtConnect$", Contained(), GetName(line), input);
  Sound("Connect");
  RemoveObject();
}

/* Produkteigenschaften */
public func IsAnvilProduct() { return(1); }
public func IsAdvancedProduct() { return(1); }
public func IsKit() { return(1); }

//Info menu
/* public func ContextInfo(pClonk){
	[$TxtLineInfo$|Image=CXIN]
	CreateMenu(CXIN,pClonk,this(),4);
	for(var i = 0; GetDefinition(i) != 0; i++){
		var j = GetDefinition(i);
		if(DefinitionCall(j,"isLine") && j != PWRL && j != DPIP && j != SPIP){
			AddMenuItem(GetName(,j),"DoLineDesc",j,pClonk,0,GetOwner(pClonk),GetDesc(,j));
		}
	}
}

public func DoLineDesc(line,owner){
	MessageWindow(GetDesc(,line),owner,CXIN,GetName(,line));
} */

public func GetResearchBase() {return(LNKT);}