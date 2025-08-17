/*-- Neues Szenario --*/

#strict

func Initialize() {
  FindObject2(Find_ID(FALW))->AddForgeable(METL, RGBa(177, 171, 170, 0), 3, 30, 2, "Meltable");
  FindObject2(Find_ID(FALW))->AddForgeable(CPIG, RGBa(170, 79, 66, 0), 4, 36, 3, "Meltable");
  return(1);
}
