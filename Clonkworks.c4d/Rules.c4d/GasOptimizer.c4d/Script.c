/*-- Gefangennahme --*/

#strict 2

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}

global func GasOptimizer_Limit(){
  return(800); //it starts to lag at around 1200 sometimes, change this for your scenarios if needed :)
}

protected func Limit(){
  var GasAmount = ObjectCount2(Find_Func("isGas"));
  if(GasAmount > GasOptimizer_Limit()){
    while(GasAmount > GasOptimizer_Limit()){
        RemoveObject(FindObject2(Find_Func("isGas")));
        GasAmount = ObjectCount2(Find_Func("isGas"));
    }
  }
}