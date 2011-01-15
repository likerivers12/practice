BEGIN { 
  i=0
  while ( i < FOLD_NUM - 1 ) {
    printf("\n")
    i=i+1
  }

  if (FOLD_NUM == 0) {
    i=0
    while ( i < (FOLD_CNT -1)) {
      printf("\n")
      i=i+1
    }
  }
}
###
{
  printf("%d\n", $1) 

  i=0
  while (i < FOLD_CNT - 1) {
    printf("\n")
    i=i+1
  }
}
