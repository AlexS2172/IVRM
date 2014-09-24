#ifndef FIX_TRADE_CONST_H
#define FIX_TRADE_CONST_H

// ExecTransType
#define		ETT_NEW							'0'
#define		ETT_CANCEL						'1'
#define		ETT_CORRECT						'2'
#define		ETT_STATUS						'3'

// OrdStatus
/*#define		OS_NEW							'0'*/
#define		OS_PARTIALLY_FILLED				'1'
#define		OS_FILLED						'2'
/*#define		OS_DONE_FOR_DAY					'3'
#define		OS_CANCELED						'4'
#define		OS_REPLACED						'5'
#define		OS_PENDING_CANCEL_REPLACE		'6'
#define		OS_STOPPED						'7'
#define		OS_REJECTED						'8'
#define		OS_SUSPENDED					'9'
#define		OS_PENDING_NEW					'A'
#define		OS_CALCULATED					'B'
#define		OS_EXPIRED						'C'*/

// Side
#define		SIDE_BUY						'1'
#define		SIDE_SELL						'2'
/*#define		SIDE_BUY_MINUS					'3'
#define		SIDE_SELL_PLUS					'4'
#define		SIDE_SELL_SHORT					'5'
#define		SIDE_SELL_SHORT_EXEMPT			'6'
#define		SIDE_UNDISCLOSED				'7'
#define		SIDE_CROSS						'8'*/

// Security type
#define		ST_OPTION						_T("OPT")
#define		ST_STOCK						_T("CS")
#define		ST_MUTUAL_FOND					_T("MF")
#define		ST_PREFERRED_STOCK				_T("PS")

// Put or call
#define		PC_PUT							0
#define		PC_CALL							1

#endif