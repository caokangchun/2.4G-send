
#ifndef _DRV_WATCHDOG_H_
#define _DRV_WATCHDOG_H_

#include "HAL_iwdg.h"

/* Exported macro ---------------------------------------------- */
#define _WDT_ENABLE

#ifdef _WDT_ENABLE
#define Watchdog_feed()		WDT_Refresh()
#else
#define Watchdog_feed()
#endif

/* Exported functions ------------------------------------------ */
void WDT_Configuration(uint16_t us_wdt);
void WDT_Refresh(void);


#endif /* end of _DRV_WATCHDOG_H_ */

