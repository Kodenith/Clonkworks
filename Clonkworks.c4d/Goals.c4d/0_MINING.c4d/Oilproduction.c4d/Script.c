/*-- �lf�rderung --*/

#strict
#include OREM

public func PlayerHasSold (plr, obj) {     
  if (GetID (obj) == OBRL) Local (plr) += obj -> OBRL::CalcValue();
} 

public func IsFulfilled()
{
  // Erz noch nicht gez�hlt?
  var OreCnt = GetMaterialCount(Material("Oil"));
  if (OreCnt == -1) return();
  // noch zu viel?
  if (OreCnt >= 150) return();
  // noch Erzst�cke abzubauen?
  return(!ObjectCount(OBRL)&&!ObjectCount2(Find_Func("IsOilGeyser")) );
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

func SellID () { return (OBRL); }

protected func Activate(iPlayer)
{
  var oreTotal = ObjectCount2(Find_ID(OBRL));
  var oreOutside = ObjectCount2(Find_ID(OBRL), Find_NoContainer());
  var deposits = ObjectCount2(Find_Func("IsOilGeyser"));
  var Units = Max(GetMaterialCount(Material("Oil")) - 150, 0) / 200;
  var Sold = Local(iPlayer);
  var HighestSold = GetHighestSold();
  
  var pClonk = GetCursor(iPlayer);
  
  CreateMenu(GetID(),pClonk,this(),0,GetName(),0,1);
  AddMenuItem(GetDesc(),0,0,pClonk);
  AddMenuItem("$TxtTasklist$",0,0,pClonk);
  if(oreTotal)
	AddMenuItem(Format("$TxtChunks1$",oreTotal,oreOutside),0,_MRK,pClonk);
  else AddMenuItem("$TxtChunks2$",0,_MRK,pClonk,0,0,0,2,1);
  
  if(Units && Units != -1)
	AddMenuItem(Format("$TxtOre1$",Units),0,_MRK,pClonk);
  else AddMenuItem("$TxtOre2$",0,_MRK,pClonk,0,0,0,2,1);
  
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
