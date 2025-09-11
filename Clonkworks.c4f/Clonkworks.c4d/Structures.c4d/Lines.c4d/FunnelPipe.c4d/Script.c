/*--- Zuflussrohr ---*/

#strict

protected func Initialize()
{
  Local(0) = 2;
  Local(1) = 3;
	
  SetAction("Connect");  
  SetVertex(0, 0, GetX()); SetVertex(0, 1, GetY());
  SetVertex(1, 0, GetX()); SetVertex(1, 1, GetY());
  SetPosition(0, 0, this());
}

protected func Transfer()
{
  if(Contained()) Exit();
  var from = GetActionTarget(0);
  var to = GetActionTarget(1);
  
  if(from && to){
	  if(GetAction(to) eq "DoorOpen") return(0); //im doing this so it doesnt get stuck on the open animation.
	  if(GetAction(from) eq "DoorOpen") return(0); // this too, apparently i forgot to do this previosuly
	  var MoveItem = FindObject2(Find_Container(from), Find_OCF(OCF_Collectible), Sort_Random());
	  if(GetOCF(to) & OCF_Container){
		  if(ContentsCount(,to) < GetDefCoreVal("CollectionLimit", "DefCore", GetID(to)) || GetDefCoreVal("CollectionLimit", "DefCore", GetID(to)) == 0){
			  if(MoveItem != this())
			  Enter(to, MoveItem);
		  }
	  }
	  else if(GetOCF(to) & OCF_Collectible){
		  var cont;
		  cont = Contained(to);
		  if(GetOCF(cont) & OCF_Alive) return(0);
		  if(cont){
			 if(ContentsCount(,cont) < GetDefCoreVal("CollectionLimit", "DefCore", GetID(cont)) || GetDefCoreVal("CollectionLimit", "DefCore", GetID(cont)) == 0){
			  if(MoveItem != this())
			  Collect(cont, MoveItem);
			 }
		  }
		  else{
		  if(GetID(to) == FNKT) return(0);
		  Exit(MoveItem);
		  SetPosition(GetX(to), GetY(to), MoveItem);
		  }
	  }
  }
}

public func LineBreak(bool fNoMsg)
{
  Sound("LineBreak");
  if (!fNoMsg) BreakMessage();
}

private func BreakMessage()
{
  var pPumpTarget = GetActionTarget(0);
  if (GetID(pPumpTarget) != FNKT)
    pPumpTarget = GetActionTarget(1);
  Message("$TxtLinebroke$", pPumpTarget);
}

public func KitType(){ return(FNKT); }
