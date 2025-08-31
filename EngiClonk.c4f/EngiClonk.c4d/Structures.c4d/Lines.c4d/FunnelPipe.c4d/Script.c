/*--- Abflussrohr ---*/

#strict

local from;
local to;
local hasBeenConnected;

protected func Initialize()
{
  hasBeenConnected = false;
  SetAction("Connect"); 
}

private func Transfer() 
{
  if(from && to){
	  hasBeenConnected = true;
	  if(DrawParticleWire("HopperPipe", GetX(from), GetY(from), GetX(to),GetY(to),3,13,RGBa(100,100,100),RGBa(120,120,120)) != -1){
	  if(ObjectCount(,,,,,OCF_Collectible,,,from)){
		  if(GetOCF(to) & OCF_Container){
			  if(ContentsCount(,to) < GetDefCoreVal("CollectionLimit", "DefCore", GetID(to))-1 || GetDefCoreVal("CollectionLimit", "DefCore", GetID(to)) == 0){
	      Enter(to,FindObject2(Find_OCF(OCF_Collectible), Find_Container(from)));
		  ProtectedCall(to, "Collection");
			  }
		  }else{
			  var exito = FindObject2(Find_OCF(OCF_Collectible), Find_Container(from));
			  Exit(exito);
			  SetPosition(GetX(to),GetY(to), exito);
		  }
	}}
      else LineBreak(false);
  }else{
	  if(hasBeenConnected) LineBreak(false);
  }
}

public func LineBreak(bool fNoMsg)
{
  Sound("LineBreak");
  if (!fNoMsg) BreakMessage();
  RemoveObject(this());
}
  
public func BreakMessage()
{
  if (GetID(to) == LNKT) 
    Message("$TxtLinebroke$", to);
  else
    Message("$TxtLinebroke$", from);
}
