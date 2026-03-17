/*-- Neues Script --*/

#strict 2
#appendto OBRL

func Activate(pClonk){
  if(pClonk->~isClobot()){
    DoEnergy(45,pClonk);
    Sound("Electric");
    pClonk->Drink();
    ChangeDef(BARL);
    return(1);
  }
  return(_inherited(pClonk));
}