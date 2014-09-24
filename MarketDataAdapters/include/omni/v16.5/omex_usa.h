

/*     Type definitions of OMnet records. Version 16.5	*/
/*     created  Thu Jun 28 13:07:26 2001 		*/

/*     Definitions of sub structures     		*/


#if !(defined(INTLIST) && defined(QUADWORDS))
typedef   struct { char quad_word[8]; } quad_word;
#else
#define quad_word quadword
#endif



typedef struct series
{
   uint8 country_c; // country-exchange code (ISE,NYSE ...)
   uint8 market_c; //Index,Stock,Bond...
   uint8 instrument_group_c;//Future, Call Option, Put Option...
   uint8 modifier_c;//new or split
   uint16 commodity_n;
   uint16 expiration_date_n;
   int32 strike_price_i;
} series_t;



typedef struct trading_code
{
   char country_id_s [2];
   char ex_customer_s [5];
   char user_id_s [5];
} trading_code_t;



typedef struct orig_series
{
   uint8 country_c;
   uint8 market_c;
   uint8 instrument_group_c;
   uint8 modifier_c;
   uint16 commodity_n;
   uint16 expiration_date_n;
   int32 strike_price_i;
} orig_series_t;



typedef struct account
{
   char country_id_s [2];
   char ex_customer_s [5];
   char account_id_s [10];
   char filler_3_s [3];
} account_t;



typedef struct user_code
{
   char country_id_s [2];
   char ex_customer_s [5];
   char user_id_s [5];
} user_code_t;



typedef struct countersign_code
{
   char country_id_s [2];
   char ex_customer_s [5];
   char user_id_s [5];
} countersign_code_t;



typedef struct new_series
{
   uint8 country_c;
   uint8 market_c;
   uint8 instrument_group_c;
   uint8 modifier_c;
   uint16 commodity_n;
   uint16 expiration_date_n;
   int32 strike_price_i;
} new_series_t;



typedef struct party
{
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_1_s;
} party_t;



typedef struct pos_account
{
   char country_id_s [2];
   char ex_customer_s [5];
   char account_id_s [10];
   char filler_3_s [3];
} pos_account_t;



typedef struct tick_size
{
   int32 step_size_i;
   int32 lower_limit_i;
   int32 upper_limit_i;
} tick_size_t;



typedef struct transaction_type
{
   char central_module_c;
   char server_type_c;
   uint16 transaction_number_n;
} transaction_type_t;



typedef struct cl_trade_api
{
   trading_code_t trading_code;
   series_t series;
   account_t account;
   user_code_t user_code;
   countersign_code_t countersign_code;
   new_series_t new_series;
   party_t party;
   pos_account_t pos_account;
   orig_series_t orig_series;
   int32 sequence_number_i;
   int32 trade_number_i;
   int32 orig_trade_number_i;
   int32 deal_price_i;
   int32 trade_quantity_i;
   int32 deal_number_i;
   uint32 global_deal_no_u;
   int32 ext_seq_nbr_i;
   int32 ext_status_i;
   int32 rem_quantity_i;
   int32 quantity_i;
   uint64 order_number_u;
   uint32 ext_trade_number_u;
   uint32 orig_ext_trade_number_u;
   int32 residual_i;
   char created_date_s [8];
   char created_time_s [6];
   char asof_date_s [8];
   char asof_time_s [6];
   char modified_date_s [8];
   char modified_time_s [6];
   char customer_info_s [15];
   char clearing_date_s [8];
   char passthrough_s [32];
   char reserved_2_s [2];
   uint8 orig_trade_type_c;
   uint8 bought_or_sold_c;
   uint8 deal_source_c;
   uint8 open_close_req_c;
   uint8 open_close_c;
   uint8 trade_type_c;
   char reserved_1_c;
   uint8 trade_state_c;
   uint8 attention_c;
   int8 account_type_c;
   uint8 instigant_c;
   uint8 cab_price_ind_c;
   uint8 ext_trade_fee_type_c;
   uint64 nbr_held_q;
   uint64 nbr_written_q;
   uint64 total_held_q;
   uint64 total_written_q;
   int32 commission_i;
} cl_trade_api_t;



typedef struct upper_level_series
{
   uint8 country_c;
   uint8 market_c;
   uint8 instrument_group_c;
   uint8 modifier_c;
   uint16 commodity_n;
   uint16 expiration_date_n;
   int32 strike_price_i;
} upper_level_series_t;



typedef struct transaction_type_low
{
   transaction_type_t transaction_type;
} transaction_type_low_t;



typedef struct transaction_type_high
{
   transaction_type_t transaction_type;
} transaction_type_high_t;



typedef struct series_fields_used
{
   uint8 country_c;
   uint8 market_c;
   uint8 instrument_group_c;
   uint8 modifier_c;
   uint16 commodity_n;
   uint16 expiration_date_n;
   int32 strike_price_i;
} series_fields_used_t;



typedef struct partition_low
{
   uint8 country_c;
   uint8 market_c;
   uint8 instrument_group_c;
   uint8 modifier_c;
   uint16 commodity_n;
   uint16 expiration_date_n;
   int32 strike_price_i;
} partition_low_t;



typedef struct partition_high
{
   uint8 country_c;
   uint8 market_c;
   uint8 instrument_group_c;
   uint8 modifier_c;
   uint16 commodity_n;
   uint16 expiration_date_n;
   int32 strike_price_i;
} partition_high_t;



typedef struct da211
{
   char country_id_s [2];
   char ex_customer_s [5];
   uint8 bin_c;
   uint8 cst_type_c;
   char filler_3_s [3];
} da211_t;



typedef struct da206
{
   char user_id_s [5];
   char filler_1_s;
   uint16 items_n;
   struct
   {
       series_t series;
   } item [100];
} da206_t;



typedef struct give_up_member
{
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_1_s;
} give_up_member_t;



typedef struct da212
{
   uint16 commodity_n;
   uint16 depends_on_commodity_n;
   uint32 number_of_share_i;
   uint32 cash_amount_i;
} da212_t;



typedef struct da24
{
   uint8 country_c;
   int8 opra_indicator_c;
   char name_s [32];
   char exchange_short_s [4];
   char country_id_s [2];
} da24_t;



typedef struct da210
{
   series_t series;
   upper_level_series_t upper_level_series;
   int32 price_quot_factor_i;
   int32 contract_size_i;
   int32 exerc_limit_i;
   int32 bbo_volume_i;
   int32 block_order_min_size_i;
   int32 block_order_rsptime_i;
   int32 fac_order_min_size_i;
   int32 fac_order_rsptime_i;
   uint16 derivate_level_n;
   uint16 dec_in_strike_price_n;
   uint16 dec_in_contr_size_n;
   int16 spg1_low_limit_n;
   int16 spg2_low_limit_n;
   int16 spg3_low_limit_n;
   int16 spg4_low_limit_n;
   int16 spg5_low_limit_n;
   uint16 rnt_id_n;
   uint16 dec_in_premium_n;
   char base_cur_s [3];
   uint8 items2_c;
   struct
   {
       uint16 edg_low_limit_n;
   } exp_date_group [20];
   uint8 exerc_limit_unit_c;
   uint8 traded_c;
   uint16 items_n;
   struct
   {
       tick_size_t tick_size;
   } item [12];
   uint8 items_c;
   char filler_3_s [3];
   struct
   {
       tick_size_t tick_size;
   } tss_mm [12];
} da210_t;



typedef struct da203
{
   series_t series;
   int32 bbo_volume_i;
   int32 block_order_min_size_i;
   int32 block_order_rsptime_i;
} da203_t;



typedef struct da208
{
   uint8 instrument_group_c;
   char name_s [32];
   char filler_3_s [3];
} da208_t;



typedef struct da207
{
   uint8 country_c;
   uint8 market_c;
   char name_s [32];
   char filler_2_s [2];
} da207_t;



typedef struct mm_parameters
{
   uint16 tick_worse_volume_an [8];
   uint16 isemm_trade_limit_absolute_n;
   uint16 firm_trade_limit_absolute_n;
   uint16 farmm_trade_limit_absolute_n;
   uint16 step_up_after_regen_buffer_n;
   int8 isemm_trade_limit_fraction_c;
   int8 firm_trade_limit_fraction_c;
   int8 farmm_trade_limit_fraction_c;
   char filler_1_s;
} mm_parameters_t;



  typedef struct pmm_parameters
{
   uint16 derived_order_max_size_an [8];
   uint16 match_away_market_max_size_an [8];
} pmm_parameters_t;



typedef struct da202
{
   series_t series;
   char ins_id_s [32];
   int32 contract_size_i;
   char symbol_s [7];
   char month_code_c;
   char strike_code_c;
   char filler_3_s[3];
} da202_t;



typedef struct da204
{
   uint16 commodity_n;
   char com_id_s [6];
   uint16 dec_in_price_n;
   char date_release_s [8];
   char date_termination_s [8];
   char name_s [32];
   char base_cur_s [3];
   char filler_3_s [3];
   uint64 nominal_value_q;
   uint64 position_limit_q;
   uint16 day_count_n;
   uint16 days_in_interest_year_n;
   uint32 coupon_interest_i;
   uint16 coupon_settlement_days_n;
   uint8 underlying_type_c;
   uint8 price_unit_c;
   uint16 dec_in_nominal_n;
   uint8 bin_c;
   uint8 items_c;
   struct
   {
       char date_coupdiv_s [8];
       uint32 dividend_i;
   } coupon [20];
   uint8 orderbook_c;
   char cusip_s [12];
   uint8 prm_country_c;
   char prm_mm_customer_s [7];
   uint8 items2_c;
   char filler_2_s [2];
   struct
   {
       uint16 level_n;
       char filler_2_s [2];
       int32 match_interval_i;
   } fast_market_lvl [10];
} da204_t;



typedef struct broadcast_type
{
   char central_module_c;
   char server_type_c;
   uint16 transaction_number_n;
} broadcast_type_t;



typedef struct alter_combo_trans_mo133_item
{
   series_t series;
   uint8 open_close_req_c;
   char filler_3_s [3];
} alter_combo_trans_mo133_item_t;



typedef struct answer_bi8_broadcasts_sent_item1
{
   uint16 broadcast_number_n;		// should go
   uint8 country_c;
   uint8 market_c;
   uint16 items_n;
   char filler_2_s [2];			// should go
   struct
   {
       char free_text_80_s [80];
   } item2 [10];
} answer_bi8_broadcasts_sent_item1_t;



typedef struct answer_bin_da211_item
{
   da211_t da211;
} answer_bin_da211_item_t;



typedef struct answer_broker_da206_item
{
   da206_t da206;
} answer_broker_da206_item_t;



typedef struct series_next
{
   uint8 country_c;
   uint8 market_c;
   uint8 instrument_group_c;
   uint8 modifier_c;
   uint16 commodity_n;
   uint16 expiration_date_n;
   int32 strike_price_i;
} series_next_t;



typedef struct answer_combo_ob_ma133_item
{
   trading_code_t trading_code;
   int32 premium_i;
   int32 reserved_1_i;
   int32 reserved_2_i;
   uint32 quantity_u;
   uint64 order_number_u;
   uint16 time_validity_n;
   uint16 reserved_4_n;
   uint16 commodity_n;
   uint8 order_type_c;
   char message_s [100];
   char reserved_32_s [32];
   uint8 interest_order_c;
   uint8 items_c;
   char filler_3_s [3];
   struct
   {
       series_t series;
       uint16 multiplier_n;
       uint8 bid_or_ask_c;
       char filler_1_s;
   } item [10];
} answer_combo_ob_ma133_item_t;



typedef struct answer_combo_order_ma131_item
{
   trading_code_t trading_code;
   int32 premium_i;
   char ex_client_s [10];
   uint16 time_validity_n;
   int32 reserved_1_i;
   int32 reserved_2_i;
   uint64 order_number_u;
   uint32 quantity_u;
   uint16 commodity_n;
   uint16 reserved_4_n;
   uint8 order_type_c;
   uint8 interest_order_c;
   uint8 client_category_c;
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char message_s [100];
   char reserved_32_s [32];
   uint8 items_c;
   struct
   {
       series_t series;
       uint16 multiplier_n;
       uint8 bid_or_ask_c;
       uint8 open_close_req_c;
   } item [10];
} answer_combo_order_ma131_item_t;



typedef struct answer_dependencies_da212_item
{
   da212_t da212;
} answer_dependencies_da212_item_t;



typedef struct answer_each_opra_prices_ia112_item
{
   series_t series;
   int32 best_bid_premium_i;
   int32 best_ask_premium_i;
   uint32 best_bid_size_u;
   uint32 best_ask_size_u;
   int32 high_price_i;
   int32 low_price_i;
   int32 last_price_i;
   uint32 volume_u;
   uint32 turnover_u;
   uint8 country_c;
   uint8 away_market_status_c;
   char filler_2_s [2];
} answer_each_opra_prices_ia112_item_t;



typedef struct answer_exchange_da24_item
{
   da24_t da24;
} answer_exchange_da24_item_t;



typedef struct answer_inactive_combo_ma132_item
{
   trading_code_t trading_code;
   int32 premium_i;
   int32 reserved_1_i;
   int32 reserved_2_i;
   uint64 order_number_u;
   uint32 quantity_u;
   uint16 time_validity_n;
   uint16 reserved_4_n;
   uint16 commodity_n;
   uint8 order_type_c;
   uint8 interest_order_c;
   uint8 client_category_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char message_s [100];
   char reserved_32_s [32];
   uint8 items_c;
   struct
   {
       series_t series;
       uint16 multiplier_n;
       uint8 bid_or_ask_c;
       uint8 open_close_req_c;
   } item [10];
} answer_inactive_combo_ma132_item_t;



typedef struct answer_inactive_ma102_item
{
   series_t series;
   trading_code_t trading_code;
   uint64 order_number_u;
   int32 premium_i;
   uint32 quantity_u;
   uint16 time_validity_n;
   uint16 order_category_n;
   uint8 client_category_c;
   uint8 bid_or_ask_c;
   uint8 order_type_c;
   uint8 ignore_away_market_c;
   uint8 open_close_req_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char filler_1_s;
} answer_inactive_ma102_item_t;



typedef struct answer_instrument_class_da210_item
{
   da210_t da210;
} answer_instrument_class_da210_item_t;



typedef struct answer_instrument_da203_item
{
   da203_t da203;
} answer_instrument_da203_item_t;



typedef struct answer_instrument_group_da208_item
{
   da208_t da208;
} answer_instrument_group_da208_item_t;



typedef struct answer_instrument_status_item
{
   series_t series;
   uint16 state_number_n;
   char filler_2_s [2];
} answer_instrument_status_item_t;



typedef struct answer_locked_order_ma106_item
{
   series_t series;
   trading_code_t trading_code;
   uint64 order_number_u;
   int32 premium_i;
   int32 premium_guaranteed_i;
   uint32 quantity_u;
   uint16 order_category_n;
   uint16 time_validity_n;
   uint8 bid_or_ask_c;
   uint8 lock_type_c;
   uint8 order_type_c;
   uint8 client_category_c;
} answer_locked_order_ma106_item_t;



typedef struct answer_market_da207_item
{
   da207_t da207;
} answer_market_da207_item_t;



typedef struct answer_missing_trade_item
{
   cl_trade_api_t cl_trade_api;
} answer_missing_trade_item_t;



typedef struct answer_mm_exposure_limit_ua103_item
{
   series_t series;
   uint32 curtailment_trading_threshold_u;
   uint16 curtailment_spread_factor_n;
   char filler_2_s [2];
   int32 reserved_i;
} answer_mm_exposure_limit_ua103_item_t;



typedef struct answer_mm_parameters_ua101_item
{
   series_t series;
   mm_parameters_t mm_parameters;
   uint8 expiration_group_c;
   uint8 strike_price_group_c;
   char filler_2_s [2];
} answer_mm_parameters_ua101_item_t;



typedef struct answer_ob_ma103_item
{
   series_t series;
   trading_code_t trading_code;
   uint64 order_number_u;
   int32 premium_i;
   uint32 quantity_u;
   uint32 open_quantity_u;
   uint16 isemm_trade_limit_absolute_n;
   uint16 firm_trade_limit_absolute_n;
   uint16 farmm_trade_limit_absolute_n;
   uint16 time_validity_n;
   uint16 order_category_n;
   uint8 bid_or_ask_c;
   uint8 lock_type_c;
   uint8 order_type_c;
   uint8 client_category_c;
   char filler_2_s [2];
} answer_ob_ma103_item_t;



typedef struct answer_opra_prices_ia111_item
{
   series_t series;
   int32 best_bid_premium_i;
   int32 best_ask_premium_i;
   uint32 best_bid_size_u;
   uint32 best_ask_size_u;
   int32 high_price_i;
   int32 low_price_i;
   int32 last_price_i;
   uint32 volume_u;
   uint32 turnover_u;
   uint8 country_ask_c;
   uint8 country_bid_c;
   uint8 country_last_c;
   uint8 country_low_c;
   uint8 country_high_c;
   uint8 away_market_status_ask_c;
   uint8 away_market_status_bid_c;
   uint8 away_market_status_last_c;
   uint8 away_market_status_low_c;
   uint8 away_market_status_high_c;
   char filler_2_s [2];
} answer_opra_prices_ia111_item_t;



typedef struct answer_order_ma101_item
{
   series_t series;
   trading_code_t trading_code;
   uint64 order_number_u;
   int32 premium_i;
   uint32 quantity_u;
   uint16 time_validity_n;
   uint16 order_category_n;
   uint8 client_category_c;
   uint8 bid_or_ask_c;
   uint8 order_type_c;
   uint8 ignore_away_market_c;
   uint8 lock_type_c;
   uint8 open_close_req_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
} answer_order_ma101_item_t;



typedef struct answer_pmm_parameters_ua102_item
{
   series_t series;
   mm_parameters_t mm_parameters;
   pmm_parameters_t pmm_parameters;
   uint8 expiration_group_c;
   uint8 strike_price_group_c;
   char filler_2_s [2];
} answer_pmm_parameters_ua102_item_t;



typedef struct answer_prices_ia101_item
{
   series_t series;
   uint32 best_bid_size_u;
   uint32 best_ask_size_u;
   uint32 time_received_bid_u;
   uint32 time_received_ask_u;
   uint32 volume_u;
   uint32 turnover_u;
   uint32 open_balance_u;
   int32 best_bid_premium_i;
   int32 best_ask_premium_i;
   int32 closing_price_i;
   int32 opening_price_i;
   int32 high_price_i;
   int32 low_price_i;
   int32 last_price_i;
   uint16 fast_market_level_n;
   uint8 series_status_c;
   uint8 country_bid_c;
   uint8 country_ask_c;
   char filler_3_s [3];
} answer_prices_ia101_item_t;



typedef struct answer_quote_ma104_item
{
   series_t series;
   trading_code_t trading_code;
   uint64 order_number_u;
   int32 premium_i;
   uint32 quantity_u;
   uint32 open_quantity_u;
   uint16 isemm_trade_limit_absolute_n;
   uint16 firm_trade_limit_absolute_n;
   uint16 farmm_trade_limit_absolute_n;
   uint16 step_up_buffer_n;
   uint16 number_of_tick_down_levels_n;
   uint8 bid_or_ask_c;
   char filler_1_s;
} answer_quote_ma104_item_t;



typedef struct answer_rotation_ma114_item
{
   series_t series;
   int32 opening_price_i;
   uint32 volume_u;
   uint32 volume_left_market_orders_u;
   uint8 series_status_c;
   uint8 rotation_reason_c;
   char filler_2_s [2];
} answer_rotation_ma114_item_t;



typedef struct answer_series_da202_item
{
   da202_t da202;
} answer_series_da202_item_t;



typedef struct answer_trade_item
{
   cl_trade_api_t cl_trade_api;
} answer_trade_item_t;



typedef struct answer_trading_state_da245_item
{
   char state_name_s [20];
   uint16 state_number_n;
   uint16 fast_market_level_n;
   uint8 type_of_matching_c;
   uint8 trading_end_c;
   uint8 price_quotation_required_c;
   uint8 market_orders_allowed_c;
   uint8 fill_kill_allowed_c;
   uint8 edited_ob_changes_avail_c;
   uint8 ob_changes_avail_c;
   char filler_1_s;
} answer_trading_state_da245_item_t;



typedef struct answer_underlying_da204_item
{
   da204_t da204;
} answer_underlying_da204_item_t;



typedef struct answer_underlying_prices_ia121_item
{
   int32 best_bid_premium_i;
   int32 best_ask_premium_i;
   int32 closing_price_i;
   int32 opening_price_i;
   int32 high_price_i;
   int32 low_price_i;
   int32 last_price_i;
   uint32 volume_u;
   uint32 turnover_u;
   uint16 commodity_n;
   uint8 country_c;
   uint8 nasdaq_upc_indicator_c;
   uint8 nasdaq_short_sale_bid_tick_c;
   uint8 originator_id_c;
   char filler_2_s [2];
} answer_underlying_prices_ia121_item_t;



typedef struct answer_underlying_status_us_item
{
   uint16 commodity_n;
   uint8 underlying_trading_status_c;
   char filler_1_s;
} answer_underlying_status_us_item_t;



typedef struct answer_user_type_info_item
{
   transaction_type_t transaction_type;
   uint8 trans_or_bdx_c;
   char filler_3_s [3];
} answer_user_type_info_item_t;



typedef struct away_market_flag_update_us_item
{
   uint16 commodity_n;
   uint8 country_c;
   uint8 ignore_away_market_c;
} away_market_flag_update_us_item_t;



typedef struct best_bid_offer_bo101_item
{
   series_t series;
   int32 best_bid_premium_i;
   int32 best_ask_premium_i;
   uint32 best_bid_size_u;
   uint32 best_ask_size_u;
} best_bid_offer_bo101_item_t;



typedef struct best_bid_offer_extended_bo102_item
{
   series_t series;
   int32 best_bid_premium_i;
   int32 best_ask_premium_i;
   uint32 best_bid_size_u;
   uint32 best_ask_size_u;
   uint32 bid_market_order_size_u;
   uint32 ask_market_order_size_u;
   uint16 mm_number_n;
   char filler_2_s [2];
} best_bid_offer_extended_bo102_item_t;



typedef struct combo_ob_changes_bo131_item
{
   series_t series;
   uint16 multiplier_n;
   uint8 bid_or_ask_c;
   char filler_1_s;
} combo_ob_changes_bo131_item_t;



typedef struct combo_ob_member_bo132_item
{
   series_t series;
   uint16 multiplier_n;
   uint8 bid_or_ask_c;
   uint8 open_close_req_c;
} combo_ob_member_bo132_item_t;



typedef struct combo_order_trans_mo131_item
{
   series_t series;
   uint16 multiplier_n;
   uint8 bid_or_ask_c;
   uint8 open_close_req_c;
} combo_order_trans_mo131_item_t;



typedef struct deal_user_bd111_item
{
   uint64 order_number_u;
   uint32 quantity_u;
   uint8 bid_or_ask_c;
   char filler_3_s [3];
} deal_user_bd111_item_t;



typedef struct whose
{
   trading_code_t trading_code;
   char ex_client_s [10];
   char filler_2_s [2];
} whose_t;



typedef struct execute_combo_trans_mo136_item
{
   series_t series;
   uint8 open_close_req_c;
   char filler_2_s [2];
   char filler_1_s;
} execute_combo_trans_mo136_item_t;



typedef struct general_message_item
{
   char text_line_s [80];
} general_message_item_t;



typedef struct instrument_status_info_item
{
   series_t series;
   uint16 minutes_to_state_change_n;
   uint16 state_number_n;
   char warning_msg_s [80];
} instrument_status_info_item_t;



typedef struct opra_all_best_bid_offer_bo104_item
{
   series_t series;
   int32 best_bid_premium_i;
   int32 best_ask_premium_i;
   uint32 best_bid_size_u;
   uint32 best_ask_size_u;
   int32 reserved_1_i;
   int32 reserved_2_i;
   uint8 country_c;
   uint8 away_market_status_c;
   char filler_2_s [2];
} opra_all_best_bid_offer_bo104_item_t;



typedef struct opra_best_bid_offer_bo103_item
{
   series_t series;
   int32 best_bid_premium_i;
   int32 best_ask_premium_i;
   uint32 best_bid_size_u;
   uint32 best_ask_size_u;
   int32 reserved_1_i;
   int32 reserved_2_i;
   uint8 country_bid_c;
   uint8 country_ask_c;
   uint8 away_market_status_bid_c;
   uint8 away_market_status_ask_c;
} opra_best_bid_offer_bo103_item_t;



typedef struct search_series
{
   uint8 country_c;
   uint8 market_c;
   uint8 instrument_group_c;
   uint8 modifier_c;
   uint16 commodity_n;
   uint16 expiration_date_n;
   int32 strike_price_i;
} search_series_t;



typedef struct quote_trans_mo106_item
{
   series_t series;
   int32 premium_i;
   uint32 quantity_u;
   uint32 open_quantity_u;
   uint16 step_up_buffer_n;
   uint8 bid_or_ask_c;
   char filler_1_s;
} quote_trans_mo106_item_t;



typedef struct rectify_trade_ise_item
{
   int32 trade_quantity_i;
   uint8 open_close_req_c;
   int8 account_type_c;
   char customer_info_s [15];
   char sub_account_id_s [5];
   char cmta_member_s [5];
   char filler_1_s;
} rectify_trade_ise_item_t;



typedef struct rotation_request_mo113_mq114_item
{
   series_t series;
   uint8 ignore_away_market_c;
   char filler_3_s [3];
} rotation_request_mo113_mq114_item_t;



typedef struct series_status_change_bi103_item
{
   series_t series;
   int32 opening_price_i;
   uint32 volume_u;
   uint16 fast_market_level_n;
   uint8 series_status_c;
   char filler_1_s;
} series_status_change_bi103_item_t;



typedef struct set_mm_exposure_limit_ui103_item
{
   series_t series;
   uint32 curtailment_trading_threshold_u;
   uint16 curtailment_spread_factor_n;
   char filler_2_s [2];
   int32 reserved_i;
} set_mm_exposure_limit_ui103_item_t;



typedef struct set_mm_parameters_ui101_item
{
   series_t series;
   mm_parameters_t mm_parameters;
   uint8 expiration_group_c;
   uint8 strike_price_group_c;
   char filler_2_s [2];
} set_mm_parameters_ui101_item_t;



typedef struct set_pmm_parameters_ui102_item
{
   series_t series;
   mm_parameters_t mm_parameters;
   pmm_parameters_t pmm_parameters;
   uint8 expiration_group_c;
   uint8 strike_price_group_c;
   char filler_2_s [2];
} set_pmm_parameters_ui102_item_t;





/*  TRANSACTION TYPE = MO133  */

typedef struct alter_combo_trans_mo133
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   uint8 order_type_c;
   uint8 interest_order_c;
   uint8 client_category_c;
   char ex_client_s [10];
   uint16 reserved_4_n;
   int32 reserved_1_i;
   int32 reserved_2_i;
   int32 premium_i;
   uint64 order_number_u;
   uint32 quantity_u;
   uint16 time_validity_n;
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char filler_2_s [2];
   char reserved_32_s [32];
   uint8 items_c;
   char filler_3_3_s [3];
   alter_combo_trans_mo133_item_t item [10];
} alter_combo_trans_mo133_t;


/*  TRANSACTION TYPE: MO103 */

typedef struct alter_trans_mo103
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   char filler_3_s [3];
   uint64 order_number_u;
   uint32 quantity_u;
   uint16 time_validity_n;
   uint8 ignore_away_market_c;
   uint8 bid_or_ask_c;
   uint8 open_close_req_c;
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char filler_2_s [2];
   char filler_1_s;
} alter_trans_mo103_t;


/*   TRANSACTION TYPE = UA11  */

typedef struct answer_bi8_broadcasts_sent
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   char filler_1_s;
   uint8 items_c;
   answer_bi8_broadcasts_sent_item1_t item1 [50];
} answer_bi8_broadcasts_sent_t;


/*   TRANSACTION TYPE = DA211  */

typedef struct answer_bin_da211
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_bin_da211_item_t item [500];
} answer_bin_da211_t;


/*   TRANSACTION TYPE = DA206  */

typedef struct answer_broker_da206
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_1_s;
   uint16 items_n;
   answer_broker_da206_item_t item [50];
} answer_broker_da206_t;


/*   TRANSACTION TYPE = UA12  */

typedef struct answer_business_date
{
   transaction_type_t transaction_type;
   char omex_version_s [16];
   char business_date_s [8];
} answer_business_date_t;


/*  TRANSACTION TYPE = MA133  */

typedef struct answer_combo_ob_ma133
{
   transaction_type_t transaction_type;
   series_next_t series_next;
   uint64 order_number_next_u;
   uint8 orderbook_next_c;
   char filler_1_s;
   uint16 items_n;
   answer_combo_ob_ma133_item_t item [177];
} answer_combo_ob_ma133_t;


/*  TRANSACTION TYPE = MA131  */

typedef struct answer_combo_order_ma131
{
   transaction_type_t transaction_type;
   series_next_t series_next;
   uint64 order_number_next_u;
   uint8 orderbook_next_c;
   uint8 bid_or_ask_next_c;
   uint16 items_n;
   answer_combo_order_ma131_item_t item [165];
} answer_combo_order_ma131_t;


/*   TRANSACTION TYPE = DA212  */

typedef struct answer_dependencies_da212
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_dependencies_da212_item_t item [1000];
} answer_dependencies_da212_t;


/*  TRANSACTION TYPE: IA111 */

typedef struct answer_each_opra_prices_ia112
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_each_opra_prices_ia112_item_t item [1000];
} answer_each_opra_prices_ia112_t;


/*   TRANSACTION TYPE = DA24  */

typedef struct answer_exchange_da24
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_exchange_da24_item_t item [100];
} answer_exchange_da24_t;


/*  TRANSACTION TYPE = MA132  */

typedef struct answer_inactive_combo_ma132
{
   transaction_type_t transaction_type;
   series_next_t series_next;
   uint64 order_number_next_u;
   uint8 orderbook_next_c;
   uint8 bid_or_ask_next_c;
   uint16 items_n;
   answer_inactive_combo_ma132_item_t item [150];
} answer_inactive_combo_ma132_t;


/*   TRANSACTION TYPE = MA102  */

typedef struct answer_inactive_ma102
{
   transaction_type_t transaction_type;
   series_next_t series_next;
   uint64 order_number_next_u;
   uint16 items_n;
   int8 orderbook_next_c;
   uint8 bid_or_ask_next_c;
   answer_inactive_ma102_item_t item [700];
} answer_inactive_ma102_t;


/*   TRANSACTION TYPE = DA210 */

typedef struct answer_instrument_class_da210
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_instrument_class_da210_item_t item [100];
} answer_instrument_class_da210_t;


/*   TRANSACTION TYPE = DA203 */

typedef struct answer_instrument_da203
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint8 items_c;
   char filler_1_s;
   answer_instrument_da203_item_t item [100];
} answer_instrument_da203_t;


/*   TRANSACTION TYPE = DA208  */

typedef struct answer_instrument_group_da208
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_instrument_group_da208_item_t item [100];
} answer_instrument_group_da208_t;


/*   TRANSACTION TYPE = UA15  */

typedef struct answer_instrument_status
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_instrument_status_item_t item [1000];
} answer_instrument_status_t;


/*   TRANSACTION TYPE = MA106  */

typedef struct answer_locked_order_ma106
{
   transaction_type_t transaction_type;
   series_next_t series_next;
   uint64 order_number_next_u;
   uint8 bid_or_ask_next_c;
   int8 orderbook_next_c;
   uint16 items_n;
   answer_locked_order_ma106_item_t item [1000];
} answer_locked_order_ma106_t;


/*   TRANSACTION TYPE = DA207 */

typedef struct answer_market_da207
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_market_da207_item_t item [100];
} answer_market_da207_t;


/*   TRANSACTION TYPE = CA1  */

typedef struct answer_missing_trade
{
   transaction_type_t transaction_type;
   char filler_2_s [2];
   uint16 items_n;
   answer_missing_trade_item_t item [190];
} answer_missing_trade_t;


/*  TRANSACTION TYPE = UA103 */

typedef struct answer_mm_exposure_limit_ua103
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_mm_exposure_limit_ua103_item_t item [1000];
} answer_mm_exposure_limit_ua103_t;


/*  TRANSACTION TYPE: UA101 */

typedef struct answer_mm_parameters_ua101
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_mm_parameters_ua101_item_t item [900];
} answer_mm_parameters_ua101_t;


/*   TRANSACTION TYPE = MA103  */

typedef struct answer_ob_ma103
{
   transaction_type_t transaction_type;
   series_next_t series_next;
   uint64 order_number_next_u;
   uint8 bid_or_ask_next_c;
   char filler_1_s;
   uint16 items_n;
   answer_ob_ma103_item_t item [1000];
} answer_ob_ma103_t;


/*  TRANSACTION TYPE: IA111 */

typedef struct answer_opra_prices_ia111
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_opra_prices_ia111_item_t item [1000];
} answer_opra_prices_ia111_t;


/*   TRANSACTION TYPE = MA101  */

typedef struct answer_order_ma101
{
   transaction_type_t transaction_type;
   series_next_t series_next;
   uint64 order_number_next_u;
   uint16 items_n;
   int8 orderbook_next_c;
   uint8 bid_or_ask_next_c;
   answer_order_ma101_item_t item [700];
} answer_order_ma101_t;


/*  TRANSACTION TYPE: UA102 */

typedef struct answer_pmm_parameters_ua102
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_pmm_parameters_ua102_item_t item [400];
} answer_pmm_parameters_ua102_t;


/*  TRANSACTION TYPE: IA101  */

typedef struct answer_prices_ia101
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_prices_ia101_item_t item [840];
} answer_prices_ia101_t;


/*   TRANSACTION TYPE = MA104  */

typedef struct answer_quote_ma104
{
   transaction_type_t transaction_type;
   int8 orderbook_next_c;
   char filler_3_s [3];
   series_next_t series_next;
   uint64 order_number_next_u;
   uint8 bid_or_ask_next_c;
   char filler_1_s;
   uint16 items_n;
   answer_quote_ma104_item_t item [1000];
} answer_quote_ma104_t;


/*  TRANSACTION TYPE: MA114 */

typedef struct answer_rotation_ma114
{
   transaction_type_t transaction_type;
   uint16 items_n;
   char filler_2_s [2];
   answer_rotation_ma114_item_t item [100];
} answer_rotation_ma114_t;


/*   TRANSACTION TYPE = DA202  */

typedef struct answer_series_da202
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_series_da202_item_t item [1000];
} answer_series_da202_t;


/*   TRANSACTION TYPE = CA2, CA13  */

typedef struct answer_trade
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16 segment_number_n;
   uint16 items_n;
   answer_trade_item_t item [190];
} answer_trade_t;


/*   TRANSACTION TYPE = DA245  */

typedef struct answer_trading_state_da245
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_trading_state_da245_item_t item [100];
} answer_trading_state_da245_t;


/*   TRANSACTION TYPE = DA204  */

typedef struct answer_underlying_da204
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint8 items_c;
   char filler_1_s;
   answer_underlying_da204_item_t item [100];
} answer_underlying_da204_t;


/*  TRANSACTION TYPE: IA121 */

typedef struct answer_underlying_prices_ia121
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_underlying_prices_ia121_item_t item [1000];
} answer_underlying_prices_ia121_t;


/*  TRANSACTION TYPE: IA132 */

typedef struct answer_underlying_status_us
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   answer_underlying_status_us_item_t item [5000];
} answer_underlying_status_us_t;


/*   TRANSACTION TYPE = DA30 */

typedef struct answer_user_type_info
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   uint16 items_n;
   char ust_id_s [5];
   uint8 ext_or_int_c;
   uint8 is_trader_c;
   char filler_1_s;
   uint32 min_ord_percent_u;
   uint32 max_ord_percent_u;
   answer_user_type_info_item_t item [100];
} answer_user_type_info_t;


/*   TRANSACTION TYPE = UI1  */

typedef struct application_status
{
   transaction_type_t transaction_type;
   series_t series;
   int32 application_status_i;
} application_status_t;


/*  BROADCAST TYPE: BI131, BI133 */

typedef struct away_adm_msg_update_bi131
{
   broadcast_type_t broadcast_type;
   uint64 timestamp_q;
   char msg_s [1000];
   uint8 country_c;
   uint8 feeder_type_c;
   char filler_2_s [2];
   char reserved_8_s [8];
} away_adm_msg_update_bi131_t;


/*  BROADCAST TYPE: BI114 */

typedef struct away_market_flag_update_us
{
   broadcast_type_t broadcast_type;
   uint16 items_n;
   char filler_2_s [2];
   away_market_flag_update_us_item_t item [300];
} away_market_flag_update_us_t;


/*  BROADCAST TYPE: BO101 */

typedef struct best_bid_offer_bo101
{
   broadcast_type_t broadcast_type;
   int32 reserved_i;
   uint16 items_n;
   char filler_2_s [2];
   best_bid_offer_bo101_item_t item [50];
} best_bid_offer_bo101_t;


/*  BROADCAST TYPE: BO102 */

typedef struct best_bid_offer_extended_bo102
{
   broadcast_type_t broadcast_type;
   int32 reserved_i;
   uint16 items_n;
   char filler_2_s [2];
   best_bid_offer_extended_bo102_item_t item [30];
} best_bid_offer_extended_bo102_t;


/*  BROADCAST TYPE: BO106 */

typedef struct block_facilitation_order_bo106
{
   broadcast_type_t broadcast_type;
   series_t series;
   uint64 order_number_u;
   int32 premium_i;
   uint32 quantity_u;
   uint16 expiration_time_n;
   uint16 time_validity_n;
   uint16 order_category_n;
   uint8 bid_or_ask_c;
   uint8 expose_flag_c;
   uint8 client_category_c;
   uint8 broker_percentage_c;
   char free_text_s [15];
   char filler_3_s [3];
} block_facilitation_order_bo106_t;


/*  BROADCAST TYPE: BO112 */

typedef struct block_order_ended_bo112
{
   broadcast_type_t broadcast_type;
   series_t series;
   uint64 order_number_u;
   uint16 order_category_n;
   char filler_2_s [2];
} block_order_ended_bo112_t;


/*  TRANSACTION TYPE: MO107 */

typedef struct block_order_mo107
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   char filler_3_s [3];
   int32 premium_i;
   uint32 quantity_u;
   uint16 time_validity_n;
   uint8 bid_or_ask_c;
   uint8 client_category_c;
   uint8 open_close_req_c;
   uint8 expose_flag_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
} block_order_mo107_t;


/*  TRANSACTION TYPE: MO109 */

typedef struct block_order_response_mo109
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   char filler_3_s [3];
   uint64 order_number_u;
   int32 premium_i;
   uint32 quantity_u;
   uint16 order_category_n;
   uint8 bid_or_ask_c;
   uint8 open_close_req_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   char filler_2_s [2];
   give_up_member_t give_up_member;
} block_order_response_mo109_t;


/*  TRANSACTION TYPE: MO110 */

typedef struct cab_order_mo110
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   uint8 bid_or_ask_c;
   uint8 client_category_c;
   uint8 open_close_req_c;
   uint32 quantity_u;
   uint16 time_validity_n;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
} cab_order_mo110_t;


/*  BROADCAST TYPE: BO105 */

typedef struct underlying_best_bid_offer_bo105
{
   broadcast_type_t broadcast_type;
   series_t series;
   int32 best_ask_premium_i;
   int32 best_bid_premium_i;
   uint8 nasdaq_upc_indicator_c;
   uint8 nasdaq_short_sale_bid_tick_c;
   uint8 originator_id_c;
   char reserved_8_s [8];
   char filler_1_s;
} underlying_best_bid_offer_bo105_t;


/*  BROADCAST TYPE: BD103 */

typedef struct underlying_trade_info_bd103
{
   broadcast_type_t broadcast_type;
   series_t series;
   int32 high_price_i;
   int32 low_price_i;
   int32 last_price_i;
   uint32 volume_u;
   uint32 turnover_u;
   char reserved_8_s [8];
} underlying_trade_info_bd103_t;


/*  TRANSACTION TYPE = BO131  */

typedef struct combo_ob_changes_bo131
{
   broadcast_type_t broadcast_type;
   trading_code_t trading_code;
   int32 premium_i;
   int32 reserved_1_i;
   int32 reserved_2_i;
   uint64 order_number_u;
   uint32 quantity_u;
   uint16 time_validity_n;
   uint16 commodity_n;
   char message_s [100];
   char reserved_32_s [32];
   uint8 change_reason_c;
   uint8 order_type_c;
   uint8 interest_order_c;
   uint8 items_c;
   combo_ob_changes_bo131_item_t item [10];
} combo_ob_changes_bo131_t;


/*  TRANSACTION TYPE = BO132  */

typedef struct combo_ob_member_bo132
{
   broadcast_type_t broadcast_type;
   trading_code_t trading_code;
   int32 premium_i;
   char ex_client_s [10];
   uint16 time_validity_n;
   int32 reserved_1_i;
   int32 reserved_2_i;
   uint64 order_number_u;
   uint32 quantity_u;
   uint16 commodity_n;
   uint16 reserved_4_n;
   uint8 change_reason_c;
   uint8 order_type_c;
   uint8 interest_order_c;
   uint8 client_category_c;
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char message_s [100];
   char reserved_32_s [32];
   uint8 items_c;
   char filler_3_s [3];
   combo_ob_member_bo132_item_t item [10];
} combo_ob_member_bo132_t;


/*  TRANSACTION TYPE = MO131  */

typedef struct combo_order_trans_mo131
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   char filler_3_s [3];
   char ex_client_s [10];
   uint16 time_validity_n;
   int32 premium_i;
   int32 reserved_1_i;
   int32 reserved_2_i;
   uint32 quantity_u;
   uint16 reserved_4_n;
   uint8 order_type_c;
   uint8 interest_order_c;
   uint8 client_category_c;
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char message_s [100];
   char reserved_32_s [32];
   char filler_2_s [2];
   uint8 items_c;
   combo_order_trans_mo131_item_t item [10];
} combo_order_trans_mo131_t;


/*   BROADCAST TYPE = BD111  */

typedef struct deal_user_bd111
{
   broadcast_type_t broadcast_type;
   series_t series;
   int32 deal_price_i;
   uint32 deal_quantity_u;
   uint8 deal_source_c;
   uint8 items_c;
   char filler_2_s [2];
   deal_user_bd111_item_t item [95];
} deal_user_bd111_t;


/*  TRANSACTION TYPE = MO104,MO105,MO360  */

typedef struct delete_trans_mo104_mo105
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   char filler_3_s [3];
   uint64 order_number_u;
   whose_t whose;
   uint16 order_category_n;
   uint8 bid_or_ask_c;
   char filler_1_s;
} delete_trans_mo104_mo105_t;


/*  BROADCAST TYPE: BO116 */

typedef struct derived_order_bo116
{
   broadcast_type_t broadcast_type;
   series_t series;
   uint64 order_number_u;
   int32 premium_i;
   uint32 quantity_u;
   uint8 bid_or_ask_c;
   uint8 action_derived_order_c;
   char filler_2_s [2];
} derived_order_bo116_t;


/*   BROADCAST TYPE = BD4,BD5  */

typedef struct directed_trade
{
   broadcast_type_t broadcast_type;
   cl_trade_api_t cl_trade_api;
} directed_trade_t;


/*  TRANSACTION TYPE = MO136  */

typedef struct execute_combo_trans_mo136
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   uint8 client_category_c;
   uint8 step_up_to_bbo_c;
   uint8 step_up_odd_tick_c;
   char ex_client_s [10];
   uint16 time_validity_n;
   uint64 order_number_u;
   uint32 quantity_u;
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   uint8 items_c;
   char filler_3_s [3];
   execute_combo_trans_mo136_item_t item [10];
} execute_combo_trans_mo136_t;


/*  TRANSACTION TYPE: MO108 */

typedef struct facilitation_order_mo108
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   char filler_3_s [3];
   int32 premium_i;
   uint32 quantity_u;
   uint8 bid_or_ask_c;
   uint8 client_category_c;
   uint8 open_close_req_c;
   uint8 open_close_req_eam_c;
   uint8 broker_percentage_c;
   uint8 expose_flag_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
} facilitation_order_mo108_t;


/*   BROADCAST TYPE = BI8 */

typedef struct general_message
{
   broadcast_type_t broadcast_type;
   uint8 country_c;
   uint8 market_c;
   uint16 items_n;
   general_message_item_t item [10];
} general_message_t;


/*   BROADCAST TYPE = BI9  */

typedef struct info_heartbeat
{
   broadcast_type_t broadcast_type;
   uint8 heartbeat_interval_c;
   uint8 instance_c;
   uint8 tot_instances_c;
   char description_s [40];
   char filler_1_s;
} info_heartbeat_t;


/*   TRANSACTION TYPE = BI210 */

typedef struct instrument_class_update_bi210
{
   broadcast_type_t broadcast_type;
   uint16 chg_type_n;
   char filler_2_s [2];
   da210_t da210;
} instrument_class_update_bi210_t;


/*   BROADCAST TYPE = BI41  */

typedef struct instrument_status_info
{
   broadcast_type_t broadcast_type;
   uint16 items_n;
   char filler_2_s [2];
   instrument_status_info_item_t item [12];
} instrument_status_info_t;


/*  BROADCAST TYPE: BO115 */

typedef struct locked_order_bo115
{
   broadcast_type_t broadcast_type;
   series_t series;
   trading_code_t trading_code;
   uint64 order_number_u;
   int32 premium_i;
   int32 premium_best_own_exchange_i;
   int32 premium_far_away_exchange_i;
   uint32 quantity_u;
   uint16 quantity_at_best_premium_n;
   uint16 time_validity_n;
   uint16 order_category_n;
   uint8 country_c;
   uint8 bid_or_ask_c;
   uint8 order_type_c;
   uint8 ignore_away_market_c;
   uint8 client_category_c;
   uint8 lock_type_c;
   uint8 action_locked_order_c;
   char filler_3_s [3];
} locked_order_bo115_t;


/*  TRANSACTION TYPE = BO130  */

typedef struct ob_changes_bo130
{
   broadcast_type_t broadcast_type;
   trading_code_t trading_code;
   series_t series;
   give_up_member_t give_up_member;
   int32 premium_i;
   uint64 order_number_u;
   uint32 quantity_u;
   uint16 time_validity_n;
   uint16 order_category_n;
   uint8 order_type_c;
   uint8 client_category_c;
   uint8 ignore_away_market_c;
   uint8 change_reason_c;
   uint8 bid_or_ask_c;
   uint8 open_close_req_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
} ob_changes_bo130_t;


/*  BROADCAST TYPE: BO104 */

typedef struct opra_all_best_bid_offer_bo104
{
   broadcast_type_t broadcast_type;
   char filler_2_s [2];
   uint16 items_n;
   opra_all_best_bid_offer_bo104_item_t item [30];
} opra_all_best_bid_offer_bo104_t;


/*  BROADCAST TYPE: BO103 */

typedef struct opra_best_bid_offer_bo103
{
   broadcast_type_t broadcast_type;
   char filler_2_s [2];
   uint16 items_n;
   opra_best_bid_offer_bo103_item_t item [30];
} opra_best_bid_offer_bo103_t;


/*  BROADCAST TYPE: BD102 */

typedef struct opra_trade_info_bd102
{
   broadcast_type_t broadcast_type;
   series_t series;
   int32 high_price_i;
   int32 low_price_i;
   int32 last_price_i;
   uint32 volume_u;
   uint32 turnover_u;
   int32 reserved_1_i;
   int32 reserved_2_i;
   uint8 country_c;
   uint8 away_market_status_c;
   uint8 out_of_sequence_c;
   char filler_1_s;
} opra_trade_info_bd102_t;


/*  BROADCAST TYPE: BO107 */

typedef struct order_quote_deleted_bo107
{
   broadcast_type_t broadcast_type;
   series_t series;
   uint64 order_number_u;
   int32 premium_i;
   uint16 rem_quantity_n;
   uint16 order_category_n;
   uint8 bid_or_ask_c;
   uint8 reason_order_delete_c;
   char filler_2_s [2];
} order_quote_deleted_bo107_t;


/*  TRANSACTION TYPE: MO101 */

typedef struct order_trans_mo101
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   char filler_3_s [3];
   uint32 quantity_u;
   int32 premium_i;
   uint16 time_validity_n;
   uint8 bid_or_ask_c;
   uint8 order_type_c;
   uint8 ignore_away_market_c;
   uint8 client_category_c;
   uint8 open_close_req_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char filler_2_s [2];
   char filler_1_s;
} order_trans_mo101_t;


/*  TRANSACTION TYPE: MO111 */

typedef struct pmm_release_locked_order_mo111
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   char filler_3_s [3];
   uint64 order_number_u;
   uint8 bid_or_ask_c;
   uint8 lock_type_c;
   char filler_2_s [2];
} pmm_release_locked_order_mo111_t;


/*  TRANSACTION TYPE: MO112 */

typedef struct pmm_trade_report_trans_mo112
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   char filler_3_s [3];
   uint64 order_number_u;
   uint32 quantity_u;
   int32 premium_i;
   uint16 order_category_n;
   uint8 bid_or_ask_c;
   char filler_1_s;
} pmm_trade_report_trans_mo112_t;


/*   TRANSACTION TYPE = UQ11  */

typedef struct query_bi8_broadcasts_sent
{
   transaction_type_t transaction_type;
   uint16 segment_number_n;
   char filler_2_s [2];
} query_bi8_broadcasts_sent_t;


/*   TRANSACTION TYPE = DQ211  */

typedef struct query_bin_dq211
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_3_s [3];
} query_bin_dq211_t;


/*  TRANSACTION TYPE = DQ6 and DQ206 */

typedef struct query_broker
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_3_s [3];
} query_broker_t;


/*   TRANSACTION TYPE = UQ12  */

typedef struct query_business_date
{
   transaction_type_t transaction_type;
   char filler_4_s [4];
} query_business_date_t;


/*  TRANSACTION TYPE: IQ112 */

typedef struct query_each_opra_prices_iq112
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16 segment_number_n;
   uint8 bin_c;
   char filler_1_s;
} query_each_opra_prices_iq112_t;


/*   TRANSACTION TYPE = DQ24  */

typedef struct query_exchange_dq24
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
} query_exchange_dq24_t;


/*  TRANSACTION TYPE = MQ132  */

typedef struct query_inactive_combo_mq132
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   uint8 bin_c;
   uint8 bid_or_ask_c;
   char filler_1_s;
   uint16 order_category_n;
   char filler_2_s [2];
   whose_t whose;
   uint64 order_number_u;
} query_inactive_combo_mq132_t;


/*   TRANSACTION TYPE = MQ102  */

typedef struct query_inactive_mq102
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   char filler_3_s [3];
   whose_t whose;
   uint64 order_number_u;
   uint16 order_category_n;
   uint8 bin_c;
   uint8 bid_or_ask_c;
} query_inactive_mq102_t;


/*   TRANSACTION TYPE = DQ3, DQ17, DQ22, DQ25, DQ203, DQ2062 */

typedef struct query_instrument
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char filler_2_s [2];
} query_instrument_t;


/*  TRANSACTION TYPE = DQ10,DQ20,DQ21,DQ60,DQ110,DQ210,DQ2061  */

typedef struct query_instrument_class
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char filler_2_s [2];
} query_instrument_class_t;


/*  TRANSACTION TYPE = DQ8 and DQ208 */

typedef struct query_instrument_group
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char filler_2_s [2];
} query_instrument_group_t;


/*   TRANSACTION TYPE = UQ15  */

typedef struct query_instrument_status
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char filler_2_s [2];
} query_instrument_status_t;


/*   TRANSACTION TYPE = MQ106  */

typedef struct query_locked_order_mq106
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   whose_t whose;
   uint64 order_number_u;
   uint8 bin_c;
   uint8 bid_or_ask_c;
   char filler_1_s;
} query_locked_order_mq106_t;


/*  TRANSACTION TYPE = DQ7, DQ23 and DQ207 */

typedef struct query_market
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char filler_2_s [2];
} query_market_t;


/*   TRANSACTION TYPE = CQ1 */

typedef struct query_missing_trade
{
   transaction_type_t transaction_type;
   series_t series;
   int32 sequence_first_i;
   int32 sequence_last_i;
   char date_s [8];
} query_missing_trade_t;


/*  TRANSACTION TYPE = UQ103 */

typedef struct query_mm_exposure_limit_uq103
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char filler_2_s [2];
} query_mm_exposure_limit_uq103_t;


/*  TRANSACTION TYPE: UQ101 */

typedef struct query_mm_parameters_uq101
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char filler_2_s [2];
} query_mm_parameters_uq101_t;


/*   TRANSACTION TYPE = MQ103  */

typedef struct query_ob_mq103
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   char filler_1_s;
   char filler_2_s [2];
   uint64 order_number_u;
   uint8 bid_or_ask_c;
   char filler_3_s [3];
} query_ob_mq103_t;


/*  TRANSACTION TYPE = MQ133  */

typedef struct query_combo_ob_mq133
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   uint8 bin_c;
   uint8 bid_or_ask_c;
   char filler_1_s;
   uint16 order_category_n;
   char filler_2_s [2];
   whose_t whose;
   uint64 order_number_u;
} query_combo_ob_mq133_t;


/*  TRANSACTION TYPE = MQ131  */

typedef struct query_combo_order_mq131
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   uint8 bin_c;
   uint8 bid_or_ask_c;
   char filler_1_s;
   uint16 order_category_n;
   char filler_2_s [2];
   whose_t whose;
   uint64 order_number_u;
} query_combo_order_mq131_t;


/*   TRANSACTION TYPE = DQ212  */

typedef struct query_dependencies_dq212
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char filler_2_s [2];
} query_dependencies_dq212_t;


/*  TRANSACTION TYPE: IQ111 */

typedef struct query_opra_prices_iq111
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16 segment_number_n;
   uint8 bin_c;
   char filler_1_s;
} query_opra_prices_iq111_t;


/*   TRANSACTION TYPE = MQ101  */

typedef struct query_order_mq101
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   uint8 bin_c;
   uint8 bid_or_ask_c;
   char filler_1_s;
   uint16 order_category_n;
   char filler_2_s [2];
   whose_t whose;
   uint64 order_number_u;
} query_order_mq101_t;


/*  TRANSACTION TYPE: UQ102 */

typedef struct query_pmm_parameters_uq102
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char filler_2_s [2];
} query_pmm_parameters_uq102_t;


/*  TRANSACTION TYPE: IQ101  */

typedef struct query_prices_iq101
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   char filler_3_s [3];
   uint16 segment_number_n;
   uint8 bin_c;
   char filler_1_s;
} query_prices_iq101_t;


/*   TRANSACTION TYPE = MQ104  */

typedef struct query_quote_mq104
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   char filler_3_s [3];
   whose_t whose;
   uint64 order_number_u;
   uint8 bin_c;
   uint8 bid_or_ask_c;
   char filler_2_s [2];
} query_quote_mq104_t;


/*   TRANSACTION TYPE = DQ2, DQ9, DQ26, DQ102 and DQ202 */

typedef struct query_series
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char filler_2_s [2];
} query_series_t;


/*   TRANSACTION TYPE = CQ2  */

typedef struct query_trade
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16 segment_number_n;
   char from_date_s [8];
   char from_time_s [6];
   char to_date_s [8];
   char to_time_s [6];
   char filler_2_s [2];
} query_trade_t;


/*   TRANSACTION TYPE = DQ245 */

typedef struct query_trading_state_dq245
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char filler_2_s [2];
} query_trading_state_dq245_t;


/*   TRANSACTION TYPE = DQ4, DQ19, DQ104 and DQ204 */

typedef struct query_underlying
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char filler_2_s [2];
} query_underlying_t;


/*  TRANSACTION TYPE: IQ121 */

typedef struct query_underlying_prices_iq121
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   uint8 bin_c;
   char filler_1_s;
} query_underlying_prices_iq121_t;


/*  TRANSACTION TYPE: IQ132 */

typedef struct query_underlying_status_us
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 commodity_n;
   uint16 segment_number_n;
} query_underlying_status_us_t;


/*  TRANSACTION TYPE = DQ30  */

typedef struct query_user_type_info
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 segment_number_n;
   char filler_2_s [2];
} query_user_type_info_t;


/*  BROADCAST TYPE: BO108 */

typedef struct quote_rejection_reason_bo108
{
   broadcast_type_t broadcast_type;
   series_t series;
   uint64 order_number_u;
   int32 premium_i;
   uint32 reason_quote_rejected_u;
   uint16 quantity_n;
   uint16 open_quantity_n;
   uint8 bid_or_ask_c;
   uint8 item_number_c;
   char filler_2_s [2];
} quote_rejection_reason_bo108_t;


/*  BROADCAST TYPE: BO109 */

typedef struct quote_stepped_up_bo109
{
   broadcast_type_t broadcast_type;
   series_t series;
   uint64 order_number_u;
   int32 premium_i;
   uint32 quantity_u;
   uint16 step_up_buffer_n;
   uint8 bid_or_ask_c;
   char filler_1_s;
} quote_stepped_up_bo109_t;


/*  BROADCAST TYPE: BO110 */

typedef struct quote_tick_worse_bo110
{
   broadcast_type_t broadcast_type;
   series_t series;
   uint64 order_number_u;
   int32 premium_i;
   uint32 quantity_u;
   uint16 isemm_trade_limit_absolute_n;
   uint16 firm_trade_limit_absolute_n;
   uint16 farmm_trade_limit_absolute_n;
   uint16 step_up_buffer_n;
   uint16 tick_worse_level_n;
   uint8 bid_or_ask_c;
   uint8 reason_tick_worse_c;
} quote_tick_worse_bo110_t;


/*  TRANSACTION TYPE: MO106  */

typedef struct quote_trans_mo106
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   char filler_1_s;
   uint16 items_n;
   quote_trans_mo106_item_t item [192];
} quote_trans_mo106_t;


/* TRANSACTION TYPE = CD36  */

typedef struct rectify_trade_ise
{
   transaction_type_t transaction_type;
   series_t series;
   int32 trade_number_i;
   uint8 items_c;
   char filler_3_s [3];
   rectify_trade_ise_item_t item [100];
} rectify_trade_ise_t;


/*   TRANSACTION TYPE = CD33  */

typedef struct rectify_trade_occ_us
{
   transaction_type_t transaction_type;
   series_t series;
   int32 trade_number_i;
   char cmta_member_s [5];
   char sub_account_id_s [5];
   char customer_info_s [15];
   int8 account_type_c;
   uint8 open_close_req_c;
   char filler_1_s;
} rectify_trade_occ_us_t;


/*  TRANSACTION TYPE: MO113 or MQ114 */

typedef struct rotation_request_mo113_mq114
{
   transaction_type_t transaction_type;
   series_t series;
   uint8 orderbook_c;
   uint8 series_status_c;
   uint16 fast_market_level_n;
   char filler_2_s [2];
   uint16 items_n;
   rotation_request_mo113_mq114_item_t item [100];
} rotation_request_mo113_mq114_t;


/*  BROADCAST TYPE: BI103 */

typedef struct series_status_change_bi103
{
   broadcast_type_t broadcast_type;
   uint16 items_n;
   char filler_2_s [2];
   series_status_change_bi103_item_t item [50];
} series_status_change_bi103_t;


/*   TRANSACTION TYPE = BI202 */

typedef struct series_update_bi202
{
   broadcast_type_t broadcast_type;
   uint16 chg_type_n;
   char filler_2_s [2];
   da202_t da202;
} series_update_bi202_t;


/*   TRANSACTION TYPE = UI103 */

typedef struct set_mm_exposure_limit_ui103
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 items_n;
   char filler_2_s [2];
   set_mm_exposure_limit_ui103_item_t item [1000];
} set_mm_exposure_limit_ui103_t;


/*  TRANSACTION TYPE: UI101 */

typedef struct set_mm_parameters_ui101
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 items_n;
   char filler_2_s [2];
   set_mm_parameters_ui101_item_t item [900];
} set_mm_parameters_ui101_t;


/*   TRANSACTION TYPE = UI102 */

typedef struct set_pmm_parameters_ui102
{
   transaction_type_t transaction_type;
   series_t series;
   uint16 items_n;
   char filler_2_s [2];
   set_pmm_parameters_ui102_item_t item [400];
} set_pmm_parameters_ui102_t;


/*   BROADCAST TYPE = BI1  */

typedef struct suspend_resume_trading
{
   broadcast_type_t broadcast_type;
   uint16 commodity_n;
   uint8 on_off_c;
   char filler_1_s;
} suspend_resume_trading_t;


/*  BROADCAST TYPE: BD101 */

typedef struct trade_info_bd101
{
   broadcast_type_t broadcast_type;
   series_t series;
   int32 closing_price_i;
   int32 opening_price_i;
   int32 high_price_i;
   int32 low_price_i;
   int32 last_price_i;
   uint32 volume_u;
   uint32 turnover_u;
   uint32 open_balance_u;
   uint8 out_of_sequence_c;
   char filler_3_s [3];
   int32 reserved_i;
} trade_info_bd101_t;


/*  BROADCAST TYPE: BI132 */

typedef struct underlying_trading_status_bi132
{
   broadcast_type_t broadcast_type;
   uint16 commodity_n;
   uint8 underlying_trading_status_c;
   char filler_1_s;
   char reserved_8_s [8];
} underlying_trading_status_bi132_t;


/*   TRANSACTION TYPE = BI204 */

typedef struct underlying_update_bi204
{
   broadcast_type_t broadcast_type;
   uint16 chg_type_n;
   char filler_2_s [2];
   da204_t da204;
} underlying_update_bi204_t;



