/*-- Neues Script --*/

#strict

func Collection(){ Sound("Clonk"); }
func RejectCollect(cId,pObj){
  if(!ObjectOnConveyor(pObj)) return(1);
}