/*-- Neues Szenario --*/

#strict

func Initialize() {
  FindObject2(Find_ID(FALW))->AddForgeable(METL, RGBa(177, 171, 170, 0), 3, 2, 2, "");
  FindObject2(Find_ID(FALW))->AddForgeable(CPIG, RGBa(170, 79, 66, 0), 4, 1, 3, "");
  FindObject2(Find_ID(FALW))->AddForgeable(LTIG, RGBa(59, 71, 117, 0), 1, 3, 1, "Burnable Fuming");
  return(1);
}
