/*-- Baumaterial --*/

#strict

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}

private func AddCnv(){
  for(var i = 0; i < GetPlayerCount(); i++){
    var iPlr = GetPlayerByIndex(i);
    var Homebase, j;
    j = 0;
    while(Homebase = FindBase(iPlr,j++)){
      if(!ContentsCount(PCNV,Homebase)) CreateContents(PCNV,Homebase,1);
    }
  }
}