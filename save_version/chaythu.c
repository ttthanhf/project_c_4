#include <stdio.h>
#include <stdlib.h>

void read_day_note()
{
  FILE *f;
  f = fopen("SolarEvent.txt", "r");
  int day, month, year;
  char data[100], s[50];
  //guint year_select, month_select, day_select;
  //gtk_calendar_get_date(GTK_CALENDAR(calendar), &year_select, &month_select, &day_select);
 if( f!= NULL)
  while (fscanf(f, "%d/%d/%s", &day, &month, &data) > 0)
  {
      fgets(s,50,f);
      fgets(data,100,f);
  //  if (day == day_select && month == month_select)
    printf("%d %d %s", day, month, data);
    break;
    // in data v�o khung event
  }
  
  fclose(f);
  f = fopen("./abc/abcData.txt", "r");
  if( f!= NULL)
  while (fscanf(f, "%d %d %d %s", &day, &month, &year, &data))
  {

   // if (day == day_select && month == month_select && year == year_select)
    // in data v�o khung note
     printf("%d %d % %s", day, month, year, data);
  }
  fclose(f);
}
int main()
{
    read_day_note();
    return 0;
}
