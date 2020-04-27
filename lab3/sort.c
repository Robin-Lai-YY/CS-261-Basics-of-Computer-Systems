void quickSort(long *A, long len) {

  if (len <= 1) return;
  
  long *i = A, *j = A, *high = A + len - 1;
  long pivot = *high;  
    
  while (j < high) {
    
    if (*j <= pivot) {
      long t = *j;
      *j = *i;
      *i = t;
      i++;
    }
    j++;
  }
  
  long t = *high;
  *high = *i;
  *i = t;

  quickSort(A, i - A);
  quickSort(i + 1, len - (i - A) - 1);
}
