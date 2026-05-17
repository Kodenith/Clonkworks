/*-- Baumaterial --*/

#strict

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}

public func InitializePlayer(iPlr){
  SetPlrKnowledge(iPlr,HZMS);
}

public func Initialize(){
  for(var i = 0; i < GetPlayerCount(); i++){
    InitializePlayer(GetPlayerByIndex(i));
  }
}