/*-- Leitungsbausatz --*/

#strict

protected func Hit()
{
  Sound("RockHit*");
}

protected func Activate(clonk)
{
  [$TxtConnectline$]
  var obj = FindObject(0, 1,0,0,0, OCF_LineConstruct(), 0,0,0, obj);
  if(!obj) return(Message("$TxtNoNewLine$", this()) );
  if(!GetOCF(obj) & OCF_Container)  return(Message("$TxtNoNewLine$", this()) );
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
      if(!ConnectLine(pLine, obj) ) return(1);
    }
  }
  else
  {
    var linetype = FNPP;
    if(!linetype)
    {
      Sound("Error");
      return(Message("$TxtNoNewLine$", this()) );
    }

    pLine = CreateLine(linetype, GetOwner(), obj, this() );
  }

  return(1);
}

private func CreateLine(linetype, owner, from, to)
{
  var line = CreateObject(linetype, 0, 0, owner);
  ObjectSetAction(line, "Connect", from, to);
  Message("$TxtNewLine$", this(), GetName(line) );
  Sound("Connect");
  return(line);
}

/* Verbinden zweier Gebäude mit einer Leitung */
private func ConnectLine(line, to)
{

  var line_accept = GetOCF(to) & OCF_Container;

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
  
  ObjectSetAction(line, "Connect", t1, t2);

  Message("$TxtConnect$", Contained(), GetName(line), GetName(to) );
  Sound("Connect");
  RemoveObject();
  return(1);
}

/* Produkteigenschaften */
public func IsAnvilProduct() { return(1); }
public func IsAdvancedProduct() { return(1); }
