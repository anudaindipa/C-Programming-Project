/* ------------------------------------------------------------------
   issue_management.c  -  Issue / Allocation Management

    Data files (one record per line, fields separated by '|'):
     issues.txt    : issueID|itemCode|locID|quantity|issuedTo|date
     items.txt     : code|name|category|unit|unitPrice   (read only)
     locations.txt : id|name|deptCode|type               (read only)
     stock.txt     : itemCode|locID|quantity             (read + update)
   ------------------------------------------------------------------ 
   */

#include <stdio.h>
#include <string.h>
#include <time.h>
