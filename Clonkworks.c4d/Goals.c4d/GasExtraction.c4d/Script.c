/*-- Erzabbau --*/

#strict
#include GOAL

local Sold;

public func IsFulfilled()
{
  // noch Erzst�cke abzubauen?
  return(!ObjectCount(NCAN)&&!ObjectCount2(Find_Func("IsNaturalGasGeyser")));
}

public func IsFulfilledforPlr (plr)
{
  if (!IsFulfilled()) return ();
  var i = GetPlayerCount();
  while (i--)
    if (Local (GetPlayerByIndex (i)) > Local (plr)) return (0);
  return (1);
}

public func GetHighestSold(){
  var i = GetPlayerCount();
  var Top = 0;
  while (i--)
    if (Local(GetPlayerByIndex (i)) > Top) Top = Local(GetPlayerByIndex (i));
  return(Top);
}

func SellID () { return (NCAN); }

public func PlayerHasSold (plr, obj)
{
  if (GetID (obj) == SellID ())
  {
    Local (plr)+=GetValue(obj);
    Sold+=GetValue(obj);
  }
}

public func PlayerHasBought (plr, obj)
{
  if (GetID (obj) == SellID ())
  {
    Local (plr)-=25;
    Sold-=25;
  }
}

protected func Activate(iPlayer)
{
  var oreTotal = ObjectCount2(Find_ID(NCAN));
  var oreOutside = ObjectCount2(Find_ID(NCAN), Find_NoContainer());
  var deposits = ObjectCount2(Find_Func("IsNaturalGasGeyser"));
  var Sold = Local(iPlayer);
  var HighestSold = GetHighestSold();
  
  var pClonk = GetCursor(iPlayer);
  
  CreateMenu(GetID(),pClonk,this(),0,GetName(),0,1);
  AddMenuItem(GetDesc(),0,0,pClonk);
  AddMenuItem("$TxtTasklist$",0,0,pClonk);
  if(oreTotal)
	AddMenuItem(Format("$TxtChunks1$",oreTotal,oreOutside),0,_MRK,pClonk);
  else AddMenuItem("$TxtChunks2$",0,_MRK,pClonk,0,0,0,2,1);
  
  if(deposits)
	AddMenuItem(Format("$TxtDepo1$",deposits),0,_MRK,pClonk);
  else AddMenuItem("$TxtDepo2$",0,_MRK,pClonk,0,0,0,2,1);
  
  if(FindObject(RVLR)){
	  if(Sold < HighestSold){
		  AddMenuItem(Format("$TxtComp1$",Sold,HighestSold),0,RVLR,pClonk);
	  }else{
		  AddMenuItem(Format("$TxtComp2$",Sold),0,RVLR,pClonk);
	  }
	  
	  if(IsFulfilledforPlr(iPlayer))  AddMenuItem("$TxtComplete$",0,0,pClonk);
  }else{
	  if(IsFulfilled())  AddMenuItem("$TxtComplete$",0,0,pClonk);
  }
}
