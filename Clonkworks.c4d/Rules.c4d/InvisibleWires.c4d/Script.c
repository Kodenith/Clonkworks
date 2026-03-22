/*-- Baumaterial --*/

#strict

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}

func Initialize(){
  for(var i in FindObjects(Find_ID(WIRE))){
    i->Local(0) = 0;
    i->Local(1) = 0;
  }
}

func Destruction(){
  for(var i in FindObjects(Find_ID(WIRE))){
    i->Local(0) = 343;
    i->Local(1) = 2;
  }
}