/*-- Leitungsbausatz --*/

#strict

protected func Hit()
{
  Sound("RockHit*");
}

protected func Activate(clonk)
{
  [$TxtConnectline$]
	var ClosestWireTarget = FindObject2(Find_Distance(20), Find_OCF(OCF_Container), Find_Not( Find_OCF(OCF_Alive), Find_OCF(OCF_CrewMember)), Find_NoContainer());
	//is there a funnel pipe connected from this building? reject.
	for(var ln in FindObjects(Find_ID(DEPP))){
		if(LocalN("from", ln) == ClosestWireTarget){
			Message("$TxtNoNewLine$", this());
			Sound("Error");
			return(0);
		}
	}
	
  //is there a funnel pipe already connected to this
  for(var ln in FindObjects(Find_ID(DEPP))){
	  if(LocalN("to", ln) == this()){
		  //if so, check if close, then disconnect if true
		  if(ObjectDistance(this(), LocalN("from", ln)) < 20){
			  Message("$TxtLineRemoval$", this());
			  RemoveObject(ln);
			  Sound("Connect");
			  return(1);
		  }else if(ClosestWireTarget){
			  //if there's a container nearby, enter it ;)
			  LocalN("to", ln) = ClosestWireTarget;
			  Sound("Connect");
			  Message("$TxtConnect$", this(), GetName(ClosestWireTarget));
			  RemoveObject(this());
			  return(1);
		  }
		  return(0);
	  }
	  
	  if(LocalN("from", ln) == this()){
		  //if so, check if close, then disconnect if true
		  if(ObjectDistance(this(), LocalN("to", ln)) < 20){
			  Message("$TxtLineRemoval$", this());
			  RemoveObject(ln);
			  Sound("Connect");
			  return(1);
		  }else if(ClosestWireTarget){
			  //if there's a container nearby, enter it ;)
			  LocalN("from", ln) = ClosestWireTarget;
			  Sound("Connect");
			  Message("$TxtConnect$", this(), GetName(ClosestWireTarget));
			  RemoveObject(this());
			  return(1);
		  }
		  return(0);
	  }
  }
  
  //no? find something to connect to
  if(ClosestWireTarget){
	  var Pipe = CreateObject(DEPP);
	  LocalN("from", Pipe) = ClosestWireTarget;
	  LocalN("to", Pipe) = this();
	  Message("$TxtConnect$", this(), GetName(ClosestWireTarget));
	  Sound("Connect");
	  return(1);
  }else{
	  Message("$TxtNoNewLine$", this());
	  Sound("Error");
  }
  
  return(0);
}

public func IsAnvilProduct() { return(1); }
