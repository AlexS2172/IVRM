

/*     Type definitions of OMnet records. Version 17.1	*/
/*     created  Tue Dec  4 15:49:19 2001 		*/

/*     Definitions of sub structures     		*/


#if !(defined(INTLIST) && defined(QUADWORDS))
typedef   struct { char quad_word[8]; } quad_word;
#else
#define quad_word quadword
#endif


#include "om_inttypes.h"

typedef struct series
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} series_t;



typedef struct trading_code
{
   char country_id_s [2];
   char ex_customer_s [5];
   char user_id_s [5];
} trading_code_t;



typedef struct order_var
{
   int32_t premium_i;
   uint16_t mp_quantity_n;
   uint16_t block_n;
   uint16_t time_validity_n;
   char ex_client_s [10];
   char customer_info_s [15];
   uint8_t open_close_req_c;
   uint8_t bid_or_ask_c;
   uint8_t ext_t_state_c;
   uint8_t order_type_c;
   uint8_t stop_condition_c;
} order_var_t;



typedef struct ex_user_code
{
   char country_id_s [2];
   char ex_customer_s [5];
   char user_id_s [5];
} ex_user_code_t;



typedef struct give_up_member
{
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_1_s;
} give_up_member_t;



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
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
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



typedef struct orig_series
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} orig_series_t;



typedef struct tick_size
{
   int32_t step_size_i;
   int32_t lower_limit_i;
   int32_t upper_limit_i;
} tick_size_t;



typedef struct transaction_type
{
   char central_module_c;
   char server_type_c;
   uint16_t transaction_number_n;
} transaction_type_t;



typedef struct order
{
   series_t series;
   trading_code_t trading_code;
   order_var_t order_var;
   ex_user_code_t ex_user_code;
   give_up_member_t give_up_member;
   char exchange_info_s [16];
   uint16_t transaction_number_n;
   char filler_2_s [2];
} order_t;



typedef struct countersign
{
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_1_s;
} countersign_t;



typedef struct prop_trade_account
{
   char country_id_s [2];
   char ex_customer_s [5];
   char account_id_s [10];
} prop_trade_account_t;



typedef struct prop_deliv_account
{
   char country_id_s [2];
   char ex_customer_s [5];
   char account_id_s [10];
} prop_deliv_account_t;



typedef struct prop_pos_account
{
   char country_id_s [2];
   char ex_customer_s [5];
   char account_id_s [10];
} prop_pos_account_t;



typedef struct prop_margin_account
{
   char country_id_s [2];
   char ex_customer_s [5];
   char account_id_s [10];
} prop_margin_account_t;



typedef struct sink_account
{
   char country_id_s [2];
   char ex_customer_s [5];
   char account_id_s [10];
   char filler_3_s [3];
} sink_account_t;



typedef struct prop_origin_account
{
   char country_id_s [2];
   char ex_customer_s [5];
   char account_id_s [10];
   char filler_3_s [3];
} prop_origin_account_t;



typedef struct delivery_account
{
   char country_id_s [2];
   char ex_customer_s [5];
   char account_id_s [10];
   char filler_3_s [3];
} delivery_account_t;



typedef struct deliv_base
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} deliv_base_t;



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
   int32_t sequence_number_i;
   int32_t trade_number_i;
   int32_t orig_trade_number_i;
   int32_t deal_price_i;
   int32_t trade_quantity_i;
   int32_t deal_number_i;
   uint32_t global_deal_no_u;
   int32_t ext_seq_nbr_i;
   int32_t ext_status_i;
   int32_t rem_quantity_i;
   int32_t quantity_i;
   quad_word order_number_u;
   uint32_t ext_trade_number_u;
   uint32_t orig_ext_trade_number_u;
   int32_t residual_i;
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
   uint8_t orig_trade_type_c;
   uint8_t bought_or_sold_c;
   uint8_t deal_source_c;
   uint8_t open_close_req_c;
   uint8_t open_close_c;
   uint8_t trade_type_c;
   char reserved_1_c;
   uint8_t trade_state_c;
   uint8_t attention_c;
   char account_type_c;
   uint8_t instigant_c;
   uint8_t cab_price_ind_c;
   uint8_t ext_trade_fee_type_c;
   int64_t nbr_held_q;
   int64_t nbr_written_q;
   int64_t total_held_q;
   int64_t total_written_q;
   int32_t commission_i;
   give_up_member_t give_up_member;
} cl_trade_api_t;



typedef struct upper_level_series
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} upper_level_series_t;



typedef struct fac_crossing_order
{
   uint32_t min_quantity_u;
   uint32_t exposure_time_u;
   uint16_t percentage_n;
   uint16_t max_allowed_percentage_n;
   uint8_t trading_allowed_c;
   char filler_3_s [3];
} fac_crossing_order_t;



typedef struct sol_crossing_order
{
   uint32_t min_quantity_u;
   uint32_t exposure_time_u;
   uint16_t percentage_n;
   uint16_t max_allowed_percentage_n;
   uint8_t trading_allowed_c;
   char filler_3_s [3];
} sol_crossing_order_t;



typedef struct exp_crossing_order
{
   uint32_t min_quantity_u;
   uint32_t exposure_time_u;
   uint16_t percentage_n;
   uint16_t max_allowed_percentage_n;
   uint8_t trading_allowed_c;
   char filler_3_s [3];
} exp_crossing_order_t;



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
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} series_fields_used_t;



typedef struct partition_low
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} partition_low_t;



typedef struct partition_high
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} partition_high_t;



typedef struct order_record
{
   int64_t timestamp_in_q;
   quad_word order_number_u;
   party_t party;
   order_t order;
   uint16_t total_volume_n;
   uint16_t display_quantity_n;
} order_record_t;



typedef struct status_item
{
   char field_s [32];
   char stat_description_s [80];
} status_item_t;



typedef struct ced_assignment
{
   series_t series;
   int32_t quantity_i;
   char indicator_c;
   char yyyymmdd_s [8];
   char hhmmss_s [6];
   char account_id_s [10];
   char country_id_s [2];
   char ex_customer_s [5];
} ced_assignment_t;



typedef struct ced_class_file_info
{
   uint16_t commodity_n;
   uint8_t type_c;
   char filler_1_s;
   int32_t offset_i;
   int32_t dec_in_spot_spread_rate_i;
   int32_t spot_spread_rate_i;
   int32_t dec_in_non_spot_spread_rate_i;
   int32_t non_spot_spread_rate_i;
   int32_t dec_in_delivery_rate_i;
   int32_t delivery_rate_i;
   int32_t dec_in_multiplier_i;
   int32_t multiplier_i;
   int32_t dec_in_current_market_value_i;
   int32_t current_market_value_i;
   int32_t dec_in_margin_interval_i;
   int32_t margin_interval_i;
   int32_t dec_in_exchange_rate_i;
   int32_t exchange_rate_i;
   int32_t currency_haircut_i;
   uint32_t dec_option_min_margin_i;
   int32_t option_min_margin_i;
   uint32_t dec_future_min_margin_i;
   int32_t future_min_margin_i;
   int32_t dec_in_risk_free_interest_i;
   int32_t risk_free_interest_i;
   int32_t dec_in_dividend_amount_i;
   int32_t dividend_amount_i;
   char ced_dividend_date_s [8];
   char ced_currency_s [2];
   char ced_symbol_s [6];
   char ced_class_group_s [5];
   char ced_product_group_s [5];
   char filler_2_s [2];
} ced_class_file_info_t;



typedef struct ced_exercise_info
{
   trading_code_t trading_code;
   series_t series;
   int32_t rqst_type_i;
   int32_t sequence_number_i;
   int32_t quantity_i;
   int32_t ext_status_i;
   char confirm_reject_c;
   char yyyymmdd_s [8];
   char hhmmss_s [6];
   char account_id_s [10];
   char customer_info_s [15];
   int32_t total_ex_day_i;
   int32_t history_ex_i;
} ced_exercise_info_t;



typedef struct ced_theoretical_values
{
   series_t series;
   int32_t mark_price_i;
   int32_t down_side_5_i;
   int32_t down_side_4_i;
   int32_t down_side_3_i;
   int32_t down_side_2_i;
   int32_t down_side_1_i;
   int32_t up_side_5_i;
   int32_t up_side_4_i;
   int32_t up_side_3_i;
   int32_t up_side_2_i;
   int32_t up_side_1_i;
   uint16_t dec_in_premium_n;
   char down_side_5_sign_c;
   char down_side_4_sign_c;
   char down_side_3_sign_c;
   char down_side_2_sign_c;
   char down_side_1_sign_c;
   char up_side_5_sign_c;
   char up_side_4_sign_c;
   char up_side_3_sign_c;
   char up_side_2_sign_c;
   char up_side_1_sign_c;
   int32_t short_opt_adjust_i;
   char short_opt_adjust_sign_c;
   char ced_symbol_s [6];
   char filler_1_s;
} ced_theoretical_values_t;



typedef struct acc_gen
{
   char account_id_s [10];
   char account_no_s [10];
   char acronyme_c;
   char country_id_s [2];
   char ex_customer_s [5];
   char user_id_s [5];
   char acronyme_account_id_s [10];
   char filler_1_s;
} acc_gen_t;



typedef struct account_data
{
   account_t account;
   uint8_t acc_state_c;
   uint8_t read_access_c;
   char diary_number_s [15];
   char acc_type_s [12];
   char fee_type_s [12];
   char cust_bank_id_s [12];
   countersign_t countersign;
   prop_trade_account_t prop_trade_account;
   prop_deliv_account_t prop_deliv_account;
   prop_pos_account_t prop_pos_account;
   prop_margin_account_t prop_margin_account;
   sink_account_t sink_account;
   char modified_date_s [8];
   char modified_time_s [6];
   char created_date_s [8];
   char created_time_s [6];
   uint8_t auto_net_c;
   char risk_currency_s [3];
   char risk_cur_conv_c;
   uint8_t risk_margin_net_c;
   char investor_type_s [4];
   char nationality_s [4];
   char account_text_s [20];
   char csd_account_s [20];
   prop_origin_account_t prop_origin_account;
   char filler_1_s;
} account_data_t;



typedef struct cl_delivery_api
{
   char date_s [8];
   int32_t sequence_number_i;
   int32_t event_type_i;
   series_t series;
   int32_t original_delivery_number_i;
   int32_t original_key_number_i;
   uint8_t delivery_type_c;
   uint8_t originator_type_c;
   uint8_t delivery_state_c;
   uint8_t bought_or_sold_c;
   account_t account;
   delivery_account_t delivery_account;
   int64_t deliv_base_quantity_q;
   int32_t delivery_number_i;
   int32_t key_number_i;
   int32_t delivery_origin_i;
   int32_t class_no_i;
   char settlement_date_s [8];
   int64_t delivery_quantity_q;
   deliv_base_t deliv_base;
} cl_delivery_api_t;



typedef struct da211
{
   char country_id_s [2];
   char ex_customer_s [5];
   uint8_t bin_c;
   uint8_t cst_type_c;
   char filler_3_s [3];
} da211_t;



typedef struct da206
{
   char user_id_s [5];
   char filler_1_s;
   uint16_t items_n;
   struct
   {
       series_t series;
   } item [100];
} da206_t;



typedef struct physical_series
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} physical_series_t;



typedef struct combo_series
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} combo_series_t;



typedef struct old_series
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} old_series_t;



typedef struct cover_data_item
{
   series_t series;
   account_t account;
   uint8_t intraday_ind_c;
   char filler_3_s [3];
   uint32_t quantity_cover_u;
   uint32_t quantity_tot_cover_u;
   int32_t quantity_request_i;
   uint8_t state_c;
   char filler_2_s [2];
   char filler_1_s;
   uint32_t cover_request_nbr_u;
   uint32_t sequence_nbr_u;
   char created_date_s [8];
   char created_time_s [6];
   char modified_date_s [8];
   char modified_time_s [6];
   user_code_t user_code;
} cover_data_item_t;



typedef struct cover_state_item
{
   series_t series;
   uint32_t cover_request_nbr_u;
   uint32_t sequence_nbr_u;
   uint8_t state_c;
   char filler_3_s [3];
   char modified_date_s [8];
   char modified_time_s [6];
   char filler_2_s [2];
} cover_state_item_t;



typedef struct da212
{
   uint16_t commodity_n;
   uint16_t depends_on_commodity_n;
   uint32_t number_of_share_i;
   uint32_t cash_amount_i;
} da212_t;



typedef struct da24
{
   uint8_t country_c;
   char opra_indicator_c;
   char name_s [32];
   char exchange_short_s [4];
   char country_id_s [2];
} da24_t;



typedef struct da210
{
   series_t series;
   upper_level_series_t upper_level_series;
   int32_t price_quot_factor_i;
   int32_t contract_size_i;
   int32_t exerc_limit_i;
   int32_t bbo_volume_i;
   int32_t block_order_min_size_i;
   int32_t block_order_rsptime_i;
   uint16_t derivate_level_n;
   uint16_t dec_in_strike_price_n;
   uint16_t dec_in_contr_size_n;
   int16_t spg1_low_limit_n;
   int16_t spg2_low_limit_n;
   int16_t spg3_low_limit_n;
   int16_t spg4_low_limit_n;
   int16_t spg5_low_limit_n;
   uint16_t rnt_id_n;
   uint16_t dec_in_premium_n;
   char base_cur_s [3];
   uint8_t items2_c;
   struct
   {
       uint16_t edg_low_limit_n;
   } exp_date_group [20];
   uint8_t exerc_limit_unit_c;
   uint8_t traded_c;
   uint16_t items_n;
   struct
   {
       tick_size_t tick_size;
   } item [12];
   uint8_t items_c;
   char filler_3_s [3];
   struct
   {
       tick_size_t tick_size;
   } tss_mm [12];
   fac_crossing_order_t fac_crossing_order;
   sol_crossing_order_t sol_crossing_order;
   exp_crossing_order_t exp_crossing_order;
} da210_t;



typedef struct da203
{
   series_t series;
   int32_t bbo_volume_i;
   int32_t block_order_min_size_i;
   int32_t block_order_rsptime_i;
} da203_t;



typedef struct da208
{
   uint8_t instrument_group_c;
   char name_s [32];
   char filler_3_s [3];
} da208_t;



typedef struct interest
{
   int32_t risk_free_rate_i;
   int32_t period_i;
} interest_t;



typedef struct da207
{
   uint8_t country_c;
   uint8_t market_c;
   char name_s [32];
   char filler_2_s [2];
} da207_t;



typedef struct mm_parameters
{
   uint16_t tick_worse_volume_an [8];
   uint16_t isemm_trade_limit_absolute_n;
   uint16_t firm_trade_limit_absolute_n;
   uint16_t farmm_trade_limit_absolute_n;
   uint16_t step_up_after_regen_buffer_n;
   char isemm_trade_limit_fraction_c;
   char firm_trade_limit_fraction_c;
   char farmm_trade_limit_fraction_c;
   char filler_1_s;
} mm_parameters_t;



typedef struct order_no_id
{
   series_t series;
   int32_t premium_i;
   uint16_t mp_quantity_n;
   uint16_t block_n;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} order_no_id_t;



typedef struct server_partition
{
   char server_name_s [20];
   transaction_type_low_t transaction_type_low;
   transaction_type_high_t transaction_type_high;
   series_fields_used_t series_fields_used;
   partition_low_t partition_low;
   partition_high_t partition_high;
   int32_t event_type_i;
} server_partition_t;



typedef struct party_no_id
{
   series_t series;
   trading_code_t trading_code;
   quad_word order_number_u;
   int32_t premium_i;
   uint32_t sequence_number_u;
   uint16_t mp_quantity_n;
   uint16_t block_n;
   uint16_t time_validity_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
} party_no_id_t;



typedef struct confirmed_by
{
   char country_id_s [2];
   char ex_customer_s [5];
   char user_id_s [5];
} confirmed_by_t;



typedef struct account_from
{
   char country_id_s [2];
   char ex_customer_s [5];
   char account_id_s [10];
} account_from_t;



typedef struct account_to
{
   char country_id_s [2];
   char ex_customer_s [5];
   char account_id_s [10];
} account_to_t;



typedef struct pmm_parameters
{
   uint16_t derived_order_max_size_an [8];
   uint16_t match_away_market_max_size_an [8];
} pmm_parameters_t;



typedef struct orig_deal_part
{
   series_t series;
   char asof_date_s [8];
   char asof_time_s [6];
   char filler_2_s [2];
   int32_t deal_price_i;
   int32_t deal_number_i;
   int32_t deal_quantity_i;
} orig_deal_part_t;



typedef struct rectify_deal_part
{
   new_series_t new_series;
   char modified_date_s [8];
   char modified_time_s [6];
   char asof_date_s [8];
   char asof_time_s [6];
   int64_t rectify_deal_number_q;
   trading_code_t trading_code;
   ex_user_code_t ex_user_code;
   int32_t state_i;
   int32_t new_deal_price_i;
} rectify_deal_part_t;



typedef struct da202
{
   series_t series;
   char ins_id_s [32];
   int32_t contract_size_i;
   char symbol_s [7];
   char month_code_c;
   char strike_code_c;
   char filler_3_s [3];
} da202_t;



typedef struct da204
{
   uint16_t commodity_n;
   char com_id_s [6];
   uint16_t dec_in_price_n;
   char date_release_s [8];
   char date_termination_s [8];
   char name_s [32];
   char base_cur_s [3];
   char filler_3_s [3];
   int64_t nominal_value_q;
   int64_t position_limit_q;
   uint16_t day_count_n;
   uint16_t days_in_interest_year_n;
   uint32_t coupon_interest_i;
   uint16_t coupon_settlement_days_n;
   uint8_t underlying_type_c;
   uint8_t price_unit_c;
   uint16_t dec_in_nominal_n;
   uint8_t bin_c;
   uint8_t items_c;
   struct
   {
       char date_coupdiv_s [8];
       uint32_t dividend_i;
   } coupon [20];
   uint8_t orderbook_c;
   char cusip_s [12];
   uint8_t prm_country_c;
   char prm_mm_customer_s [7];
   uint8_t items2_c;
   char filler_2_s [2];
   struct
   {
       uint16_t level_n;
       char filler_2_s [2];
       int32_t match_interval_i;
   } fast_market_lvl [10];
} da204_t;



typedef struct bb_order_no_id
{
   series_t series;
   int32_t premium_i;
   uint16_t mp_quantity_n;
   uint8_t combo_mark_c;
   uint8_t bid_or_ask_c;
} bb_order_no_id_t;



typedef struct ced_daily_trade_statistics
{
   series_t series;
   uint32_t bid_price_i;
   uint32_t ask_price_i;
   uint32_t first_payed_i;
   int32_t settle_price_i;
   uint32_t last_payed_i;
   uint32_t highest_payed_i;
   uint32_t lowest_payed_i;
   uint32_t volume_today_i;
   uint32_t open_balance_u;
   int32_t volatility_i;
   int32_t underlying_price_i;
} ced_daily_trade_statistics_t;



typedef struct cl_trades
{
   series_t series;
   quad_word order_number_u;
   int32_t sequence_number_i;
   int32_t trade_number_i;
   int32_t original_trade_number_i;
   int32_t deal_price_i;
   uint16_t deal_quantity_n;
   uint16_t fee_key_n;
   char account_id_s [10];
   char customer_info_s [15];
   uint8_t bought_or_sold_c;
   uint8_t deal_source_c;
   uint8_t open_close_req_c;
   uint8_t open_close_c;
   uint8_t cancel_type_c;
   char filler_2_s [2];
   ex_user_code_t ex_user_code;
   char yymmdd_s [6];
   char hhmmss_s [6];
} cl_trades_t;



typedef struct mar_gen
{
   uint16_t trading_days_n;
   uint16_t settlement_days_n;
   char mar_id_s [5];
   uint8_t country_c;
   uint8_t market_c;
   char name_s [32];
   char ntd_id_s [5];
   char tse_id_s [12];
   char open_on_inst_c;
   char closed_today_c;
   char psp_id_s [12];
   char filler_2_s [2];
} mar_gen_t;



typedef struct mp_trade
{
   int32_t deal_price_i;
   uint16_t deal_quantity_n;
   uint8_t deal_source_c;
   uint8_t state_c;
   char broker_id_s [5];
   uint8_t client_category_c;
   uint8_t aggressive_c;
   char filler_1_s;
   uint16_t state_number_n;
   char filler_2_s [2];
   order_record_t order_record;
} mp_trade_t;



typedef struct account_status_item
{
   status_item_t status_item;
} account_status_item_t;



typedef struct answer_average_price_trade_item
{
   int32_t trade_number_i;
   int32_t quantity_i;
} answer_average_price_trade_item_t;



typedef struct average_price_trade_item
{
   int32_t trade_number_i;
} average_price_trade_item_t;



typedef struct ced_answer_assignment_proxy_item
{
   ced_assignment_t ced_assignment;
} ced_answer_assignment_proxy_item_t;



typedef struct ced_answer_class_file_info_item
{
   ced_class_file_info_t ced_class_file_info;
} ced_answer_class_file_info_item_t;



typedef struct ced_answer_missing_exbyex_proxy_item
{
   ced_exercise_info_t ced_exercise_info;
} ced_answer_missing_exbyex_proxy_item_t;



typedef struct ced_answer_theoretical_values_item
{
   ced_theoretical_values_t ced_theoretical_values;
} ced_answer_theoretical_values_item_t;



typedef struct broadcast_type
{
   char central_module_c;
   char server_type_c;
   uint16_t transaction_number_n;
} broadcast_type_t;



typedef struct store_account
{
   account_t account;
   countersign_t countersign;
   prop_trade_account_t prop_trade_account;
   prop_deliv_account_t prop_deliv_account;
   prop_pos_account_t prop_pos_account;
   prop_margin_account_t prop_margin_account;
   sink_account_t sink_account;
   prop_origin_account_t prop_origin_account;
   uint8_t acc_state_c;
   uint8_t read_access_c;
   uint8_t auto_net_c;
   uint8_t foreign_holder_c;
   char risk_cur_conv_c;
   char margin_net_c;
   char diary_number_s [15];
   char acc_type_s [12];
   char fee_type_s [12];
   char cust_bank_id_s [12];
   char risk_cur_id_s [3];
   char investor_type_s [4];
   char nationality_s [4];
   char account_text_s [20];
   char csd_account_s [20];
} store_account_t;



typedef struct account_gen_item
{
   acc_gen_t acc_gen;
} account_gen_item_t;



typedef struct add_tm_combo_item
{
   series_t series;
   uint16_t ratio_n;
   char op_if_buy_c;
   char op_if_sell_c;
} add_tm_combo_item_t;



typedef struct alter_combo_trans_mo133_item
{
   series_t series;
   uint8_t open_close_req_c;
   char filler_3_s [3];
} alter_combo_trans_mo133_item_t;



typedef struct series_new
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} series_new_t;



typedef struct answer_account_item
{
   account_t account;
   uint8_t acc_state_c;
   uint8_t read_access_c;
   char diary_number_s [15];
   char acc_type_s [12];
   char fee_type_s [12];
   char cust_bank_id_s [12];
   countersign_t countersign;
   prop_trade_account_t prop_trade_account;
   prop_pos_account_t prop_pos_account;
   prop_margin_account_t prop_margin_account;
   sink_account_t sink_account;
   char modified_date_s [8];
   char modified_time_s [6];
   char created_date_s [8];
   char created_time_s [6];
   uint8_t auto_net_c;
} answer_account_item_t;



typedef struct answer_account_ext_item
{
   account_data_t account_data;
} answer_account_ext_item_t;



typedef struct answer_account_fee_type_item
{
   char fee_type_s [12];
   char description_s [40];
} answer_account_fee_type_item_t;



typedef struct answer_account_type_item
{
   char acc_type_s [12];
   char description_s [40];
   uint8_t open_close_c;
   uint8_t transitory_c;
   uint8_t market_maker_c;
   uint8_t own_inventory_c;
   uint8_t exclusive_opening_sell_c;
   uint8_t positions_allowed_c;
   uint8_t trades_allowed_c;
   char atr_id_s [12];
   char filler_1_s;
} answer_account_type_item_t;



typedef struct answer_account_type_rule_item
{
   char atr_id_s [12];
   uint8_t create_over_api_c;
   uint8_t activate_at_reg_c;
   uint16_t account_field_no_n;
   uint8_t attribute_rule_c;
   char filler_3_s [3];
} answer_account_type_rule_item_t;



typedef struct da2
{
   series_t series;
   upper_level_series_t upper_level_series;
   int32_t contract_size_i;
   char ins_id_s [32];
   char isin_code_s [12];
   uint8_t suspended_c;
   char date_last_trading_s [8];
   char time_last_trading_s [6];
   char settlement_date_s [8];
   char start_date_s [8];
   char end_date_s [8];
   char date_delivery_start_s [8];
   char date_delivery_stop_s [8];
   uint8_t series_status_c;
} da2_t;



typedef struct answer_api_delivery_item
{
   cl_delivery_api_t cl_delivery_api;
} answer_api_delivery_item_t;



typedef struct answer_api_trade_item
{
   cl_trade_api_t cl_trade_api;
} answer_api_trade_item_t;



typedef struct answer_bi26_signals_sent_item
{
   int32_t info_type_i;
   char settlement_date_s [8];
   char clh_id_s [12];
   char sent_date_s [8];
   char sent_time_s [6];
   char filler_2_s [2];
} answer_bi26_signals_sent_item_t;



typedef struct answer_bi27_broadcasts_sent_item1
{
   uint16_t broadcast_number_n;
   uint8_t country_c;
   uint8_t market_c;
   uint16_t items_n;
   char filler_2_s [2];
   struct
   {
       char free_text_80_s [80];
   } item2 [10];
} answer_bi27_broadcasts_sent_item1_t;



typedef struct answer_bi7_signals_sent_item
{
   series_t series;
   int32_t info_type_i;
   char business_date_s [8];
   char sent_date_s [8];
   char sent_time_s [6];
   char filler_2_s [2];
} answer_bi7_signals_sent_item_t;



typedef struct answer_bi8_broadcasts_sent_item1
{
   uint16_t broadcast_number_n;
   uint8_t country_c;
   uint8_t market_c;
   uint16_t items_n;
   char filler_2_s [2];
   struct
   {
       char free_text_80_s [80];
   } item2 [10];
} answer_bi8_broadcasts_sent_item1_t;



typedef struct answer_bin_da211_item
{
   da211_t da211;
} answer_bin_da211_item_t;



typedef struct answer_broker_item
{
   char user_id_s [5];
   char filler_1_s;
   uint16_t items_n;
   struct
   {
       series_t series;
   } item [100];
} answer_broker_item_t;



typedef struct answer_broker_da206_item
{
   da206_t da206;
} answer_broker_da206_item_t;



typedef struct answer_clearing_date_item
{
   series_t series;
   char clearing_date_s [8];
   char next_clearing_date_s [8];
   char prev_clearing_date_s [8];
} answer_clearing_date_item_t;



typedef struct answer_closing_series_prices_item
{
   series_t series;
   int32_t closing_price_i;
} answer_closing_series_prices_item_t;



typedef struct answer_cl_underlying_delivery_item
{
   char created_date_s [8];
   char rejected_date_s [8];
   trading_code_t trading_code;
   physical_series_t physical_series;
   series_t series;
   int32_t bond_quotation_i;
   uint16_t dec_in_bq_n;
   uint8_t state_c;
   char filler_1_s;
} answer_cl_underlying_delivery_item_t;



typedef struct answer_combo_item
{
   combo_series_t combo_series;
   char cbs_id_s [32];
   uint8_t items_c;
   char filler_3_s [3];
   struct
   {
       series_t series;
       uint16_t ratio_n;
       char op_if_buy_c;
       char op_if_sell_c;
   } item [4];
} answer_combo_item_t;



typedef struct series_next
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} series_next_t;



typedef struct answer_combo_ob_ma133_item
{
   trading_code_t trading_code;
   int32_t premium_i;
   int32_t ul_price_low_i;
   int32_t ul_price_high_i;
   uint32_t quantity_u;
   quad_word order_number_u;
   uint16_t time_validity_n;
   uint16_t reserved_4_n;
   uint16_t commodity_n;
   uint8_t order_type_c;
   char message_s [100];
   char reserved_32_s [32];
   char ul_price_method_c;
   char filler_2_s [2];
   uint8_t interest_order_c;
   uint8_t items_c;
   struct
   {
       series_t series;
       uint16_t multiplier_n;
       uint8_t bid_or_ask_c;
       char filler_1_s;
   } item [10];
} answer_combo_ob_ma133_item_t;



typedef struct answer_combo_order_ma131_item
{
   trading_code_t trading_code;
   int32_t premium_i;
   char ex_client_s [10];
   uint16_t time_validity_n;
   int32_t ul_price_low_i;
   int32_t ul_price_high_i;
   quad_word order_number_u;
   uint32_t quantity_u;
   uint16_t commodity_n;
   uint16_t reserved_4_n;
   uint8_t order_type_c;
   uint8_t interest_order_c;
   uint8_t client_category_c;
   char ul_price_method_c;
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char message_s [100];
   char reserved_32_s [32];
   uint8_t items_c;
   char filler_3_s [3];
   struct
   {
       series_t series;
       uint16_t multiplier_n;
       uint8_t bid_or_ask_c;
       uint8_t open_close_req_c;
   } item [10];
} answer_combo_order_ma131_item_t;



typedef struct answer_commission_item
{
   series_t series;
   party_t party;
   char account_id_s [10];
   char customer_info_s [15];
   char created_date_s [8];
   char created_time_s [6];
   char user_code_s [12];
   char filler_1_s;
   int32_t commission_i;
} answer_commission_item_t;



typedef struct answer_conf_give_up_req_items_item
{
   account_t account;
   int32_t trade_quantity_i;
   uint8_t open_close_req_c;
   char customer_info_s [15];
} answer_conf_give_up_req_items_item_t;



typedef struct answer_converted_series_item
{
   uint16_t adjust_ident_n;
   char filler_2_s [2];
   int32_t contract_size_i;
   old_series_t old_series;
   new_series_t new_series;
} answer_converted_series_item_t;



typedef struct answer_cover_req_item
{
   cover_data_item_t cover_data_item;
} answer_cover_req_item_t;



typedef struct answer_cover_req_upd_item
{
   cover_state_item_t cover_state_item;
} answer_cover_req_upd_item_t;



typedef struct answer_currency_item
{
   char base_cur_s [3];
   char pri_unit_s [15];
   char sec_unit_s [15];
   char third_unit_s [15];
   char pri_not_s [5];
   char sec_not_s [5];
   char third_not_s [5];
   uint8_t acc_as_pay_c;
   uint8_t currency_format_c;
   char filler_3_s [3];
} answer_currency_item_t;



typedef struct answer_daily_trade_statistics_item
{
   series_t series;
   uint32_t bid_price_i;
   uint32_t ask_price_i;
   uint32_t first_payed_i;
   int32_t settle_price_i;
   uint32_t last_payed_i;
   uint32_t highest_payed_i;
   uint32_t lowest_payed_i;
   uint32_t volume_today_i;
   uint32_t volume_yesterday_i;
   int64_t turnaround_yesterday_u;
   int64_t turnaround_today_u;
   uint32_t open_balance_u;
   int32_t volatility_i;
} answer_daily_trade_statistics_item_t;



typedef struct answer_delivery_item
{
   char date_s [8];
   int32_t event_type_i;
   series_t series;
   account_t account;
   int32_t class_no_i;
   int64_t deliv_base_quantity_q;
   char settlement_date_s [8];
   int64_t delivery_quantity_q;
   deliv_base_t deliv_base;
} answer_delivery_item_t;



typedef struct answer_dependencies_da212_item
{
   da212_t da212;
} answer_dependencies_da212_item_t;



typedef struct answer_designated_exc_da244_item
{
   uint16_t designated_list_n;
   char des_id_s [12];
   uint8_t country_c;
   char filler_1_s;
} answer_designated_exc_da244_item_t;



typedef struct answer_diss_group_da242_item
{
   uint32_t diss_group_u;
   series_t series;
   char name_s [32];
} answer_diss_group_da242_item_t;



typedef struct answer_dsg_exchg_list_status_item
{
   series_t series;
   uint16_t designated_list_n;
   char filler_2_s [2];
} answer_dsg_exchg_list_status_item_t;



typedef struct answer_each_opra_prices_ia112_item
{
   series_t series;
   int32_t best_bid_premium_i;
   int32_t best_ask_premium_i;
   int32_t best_bid_quantity_i;
   int32_t best_ask_quantity_i;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   uint32_t volume_u;
   uint32_t turnover_u;
   uint8_t country_c;
   uint8_t away_market_status_c;
   char filler_2_s [2];
} answer_each_opra_prices_ia112_item_t;



typedef struct answer_error_msg_item
{
   trading_code_t trading_code;
   series_t series;
   account_t account;
   char created_date_s [8];
   char created_time_s [6];
   char error_operation_s [10];
   uint32_t error_id_u;
   char error_problem_s [40];
} answer_error_msg_item_t;



typedef struct answer_ever_last_item
{
   series_t series;
   uint32_t ever_last_paid_i;
   uint32_t ever_last_volume_i;
} answer_ever_last_item_t;



typedef struct answer_exchange_da24_item
{
   da24_t da24;
} answer_exchange_da24_item_t;



typedef struct answer_exchange_rate_item
{
   int32_t rate_nominal_i;
   int32_t price_quot_factor_i;
   int32_t rate_low_i;
   int32_t rate_high_i;
   uint16_t dec_in_rate_n;
   uint16_t dec_in_contr_size_n;
   char price_currency_s [3];
   char other_currency_s [3];
   char filler_2_s [2];
} answer_exchange_rate_item_t;



typedef struct answer_exercise_req_item
{
   series_t series;
   account_t account;
   char reserved_1_c;
   char reserved_2_s [2];
   char filler_1_s;
   trading_code_t trading_code;
   ex_user_code_t ex_user_code;
   char modified_date_s [8];
   char modified_time_s [6];
   char asof_date_s [8];
   char asof_time_s [6];
   int32_t quantity_i;
   int32_t trade_number_i;
   int32_t exercise_number_i;
   uint8_t state_c;
   char filler_3_s [3];
} answer_exercise_req_item_t;



typedef struct answer_extended_index_item
{
   char index_s [15];
   char last_index_s [8];
   char high_index_s [8];
   char low_index_s [8];
   char change_previous_s [8];
   char change_yesterday_s [8];
   char timestamp_dist_s [5];
   char timestamp_comp_s [5];
   char significant_s [8];
   char date_s [8];
   char trend_indicator_c;
   char filler_2_s [2];
} answer_extended_index_item_t;



typedef struct answer_extended_underlying_info_item
{
   int32_t opening_ref_price_i;
   int32_t theoretical_opening_price_i;
   int32_t theoretical_closing_price_i;
   uint32_t traded_volume_opening_u;
   uint32_t theoretical_volume_opening_u;
   uint32_t traded_volume_closing_u;
   int32_t low_price_i;
   int32_t high_price_i;
   int32_t ref_price_i;
   int32_t last_price_i;
   uint32_t volume_u;
   uint32_t turnover_u;
   uint16_t commodity_n;
   char filler_2_s [2];
   struct
   {
       int32_t bid_premium_i;
   } items1 [5];
   struct
   {
       int32_t ask_premium_i;
   } items2 [5];
   struct
   {
       uint32_t bid_demand_u;
   } items3 [5];
   struct
   {
       uint32_t ask_demand_u;
   } items4 [5];
   uint32_t ape_bid_mask_u;
   uint32_t ape_ask_mask_u;
   char isin_code_s [12];
   char last_deal_time_s [6];
   char date_s [8];
   uint8_t on_off_c;
   char trading_phase_code_c;
   char trend_indicator_c;
   char filler_3_s [3];
} answer_extended_underlying_info_item_t;



typedef struct answer_external_id_da2063_item
{
   series_t series;
   char des_exc_trading_id_s [32];
   char info_feed_id_s [32];
   char depository_id_s [32];
   uint8_t country_c;
   uint8_t depository_type_c;
   char filler_2_s [2];
} answer_external_id_da2063_item_t;



typedef struct answer_ext_instrument_item
{
   series_t series;
   int32_t min_wait_time_i;
   int32_t max_wait_time_i;
   int32_t max_time_span_i;
   int32_t min_hold_time_i;
   int32_t max_time_illegal_i;
} answer_ext_instrument_item_t;



typedef struct answer_ext_inst_class_item
{
   series_t series;
   upper_level_series_t upper_level_series;
   int32_t min_wait_time_i;
   int32_t max_wait_time_i;
   int32_t max_time_span_i;
   int32_t min_hold_time_i;
   int32_t max_time_illegal_i;
   int32_t contract_size_i;
   uint16_t derivate_level_n;
   uint16_t dec_in_strike_price_n;
   uint16_t dec_in_contr_size_n;
   uint16_t rnt_id_n;
   uint16_t dec_in_premium_n;
   uint16_t items_n;
   struct
   {
       tick_size_t tick_size;
   } item [12];
   uint16_t items_block_n;
   char filler_2_s [2];
   struct
   {
       uint16_t block_n;
   } block_size [2];
   char base_cur_s [3];
   uint8_t traded_c;
} answer_ext_inst_class_item_t;



typedef struct answer_fixing_val_item
{
   series_t series;
   int32_t fixing_value_i;
   uint16_t dec_in_fixing_n;
   char filler_2_s [2];
} answer_fixing_val_item_t;



typedef struct answer_give_up_request_item
{
   series_t series;
   account_t account;
   party_t party;
   int32_t give_up_number_i;
   int32_t trade_quantity_i;
   int32_t deal_price_i;
   int32_t trade_number_i;
   int32_t commission_i;
   uint8_t bought_or_sold_c;
   uint8_t state_c;
   char created_date_s [8];
   char created_time_s [6];
   char give_up_text_s [30];
   char asof_date_s [8];
   char asof_time_s [6];
   char orig_clearing_date_s [8];
   uint8_t old_trade_c;
   uint8_t ext_trade_fee_type_c;
   uint8_t deal_source_c;
   char filler_1_s;
} answer_give_up_request_item_t;



typedef struct answer_greeks_item
{
   series_t series;
   int32_t delta_i;
   int32_t gamma_i;
   int32_t vega_i;
   int32_t theta_i;
   int32_t rho_i;
} answer_greeks_item_t;



typedef struct answer_hedge_prices_item
{
   series_t series;
   int32_t hedge_price_i;
} answer_hedge_prices_item_t;



typedef struct answer_historic_deal_item
{
   struct
   {
       cl_trade_api_t cl_trade_api;
   } trd_part [2];
} answer_historic_deal_item_t;



typedef struct answer_inactive_item
{
   order_t order;
} answer_inactive_item_t;



typedef struct answer_inactive_combo_ma132_item
{
   trading_code_t trading_code;
   int32_t premium_i;
   int32_t ul_price_low_i;
   int32_t ul_price_high_i;
   quad_word order_number_u;
   uint32_t quantity_u;
   uint16_t time_validity_n;
   uint16_t reserved_4_n;
   uint16_t commodity_n;
   uint8_t order_type_c;
   uint8_t interest_order_c;
   uint8_t client_category_c;
   char ul_price_method_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char message_s [100];
   char reserved_32_s [32];
   uint8_t items_c;
   char filler_3_s [3];
   struct
   {
       series_t series;
       uint16_t multiplier_n;
       uint8_t bid_or_ask_c;
       uint8_t open_close_req_c;
   } item [10];
} answer_inactive_combo_ma132_item_t;



typedef struct answer_inactive_ma102_item
{
   series_t series;
   trading_code_t trading_code;
   quad_word order_number_u;
   int32_t premium_i;
   uint32_t quantity_u;
   uint16_t time_validity_n;
   uint16_t order_category_n;
   uint8_t client_category_c;
   uint8_t bid_or_ask_c;
   uint8_t order_type_c;
   uint8_t ignore_away_market_c;
   uint8_t open_close_req_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char filler_1_s;
} answer_inactive_ma102_item_t;



typedef struct answer_instrument_item
{
   series_t series;
   int32_t max_order_vol_i;
   char int_id_s [4];
   char name_s [32];
} answer_instrument_item_t;



typedef struct answer_instrument_class_item
{
   series_t series;
   upper_level_series_t upper_level_series;
   int32_t price_quot_factor_i;
   int32_t contract_size_i;
   int32_t exerc_limit_i;
   int32_t redemption_value_i;
   uint16_t derivate_level_n;
   uint16_t dec_in_strike_price_n;
   uint16_t dec_in_contr_size_n;
   uint16_t rnt_id_n;
   uint16_t dec_in_premium_n;
   uint16_t items_n;
   struct
   {
       tick_size_t tick_size;
   } item [12];
   uint16_t items_block_n;
   char filler_2_s [2];
   struct
   {
       uint16_t block_n;
   } block_size [2];
   char base_cur_s [3];
   uint8_t traded_c;
   uint8_t exerc_limit_unit_c;
   char inc_id_s [10];
   char name_s [32];
   char is_fractions_c;
   uint8_t price_format_c;
   uint8_t strike_price_format_c;
   uint8_t cabinet_format_c;
   uint8_t price_unit_premium_c;
   uint8_t price_unit_strike_c;
   char filler_3_s [3];
} answer_instrument_class_item_t;



typedef struct answer_instrument_class_da210_item
{
   da210_t da210;
} answer_instrument_class_da210_item_t;



typedef struct answer_instrument_da203_item
{
   da203_t da203;
} answer_instrument_da203_item_t;



typedef struct answer_instrument_group_item
{
   uint8_t instrument_group_c;
   char name_s [32];
   char filler_3_s [3];
} answer_instrument_group_item_t;



typedef struct answer_instrument_group_da208_item
{
   da208_t da208;
} answer_instrument_group_da208_item_t;



typedef struct answer_instrument_status_item
{
   series_t series;
   uint16_t state_number_n;
   char filler_2_s [2];
} answer_instrument_status_item_t;



typedef struct answer_inst_class_da240_item
{
   series_t series;
   int32_t price_quot_factor_i;
   int32_t contract_size_i;
   uint16_t dec_in_strike_price_n;
   uint16_t dec_in_contr_size_n;
   uint16_t rnt_id_n;
   uint16_t dec_in_premium_n;
   uint16_t items_normal_n;
   uint16_t items_fast_n;
   struct
   {
       tick_size_t tick_size;
   } tss_normal [8];
   struct
   {
       tick_size_t tick_size;
   } tss_fast [8];
   uint16_t items_block_n;
   uint8_t traded_c;
   char is_fractions_c;
   struct
   {
       uint16_t block_n;
   } block_size [2];
   uint32_t max_ord_value_u;
   uint32_t min_ord_value_u;
   char base_cur_s [3];
   uint8_t pmm_obl_type_c;
} answer_inst_class_da240_item_t;



typedef struct answer_interest_rate_item
{
   interest_t interest;
} answer_interest_rate_item_t;



typedef struct answer_in_srv_item
{
   series_t series;
   uint16_t block_n;
   uint8_t premium_levels_c;
   char trend_indicator_c;
   struct
   {
       int32_t bid_premium_i;
   } items1 [5];
   struct
   {
       int32_t ask_premium_i;
   } items2 [5];
   struct
   {
       uint32_t bid_demand_u;
   } items3 [5];
   struct
   {
       uint32_t ask_demand_u;
   } items4 [5];
   int32_t closing_price_i;
   int32_t opening_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   int32_t average_price_i;
   uint32_t volume_u;
   uint32_t turnover_u;
   int64_t turnaround_today_u;
   uint32_t open_balance_u;
   uint32_t number_of_deals_u;
   int64_t date_and_time;
} answer_in_srv_item_t;



typedef struct answer_locked_order_ma106_item
{
   series_t series;
   trading_code_t trading_code;
   quad_word order_number_u;
   int32_t premium_i;
   int32_t premium_guaranteed_i;
   uint32_t quantity_u;
   uint16_t order_category_n;
   uint16_t time_validity_n;
   uint8_t bid_or_ask_c;
   uint8_t lock_type_c;
   uint8_t order_type_c;
   uint8_t client_category_c;
} answer_locked_order_ma106_item_t;



typedef struct answer_manual_payments_item
{
   series_t series;
   account_t account;
   char settlement_date_s [8];
   int64_t amount_u;
   char invc_text_s [60];
   char currency_s [3];
   uint8_t pay_or_receive_c;
} answer_manual_payments_item_t;



typedef struct answer_margin_item
{
   series_t series;
   int64_t margin_req_u;
   char country_id_s [2];
   char ex_customer_s [5];
   char account_id_s [10];
   char filler_3_s [3];
} answer_margin_item_t;



typedef struct answer_margin_acc_item
{
   account_t account;
   char market_currency_s [3];
   char filler_1_s;
   int64_t market_margin_q;
   int64_t risk_margin_q;
   char cust_bank_id_s [12];
   char risk_currency_s [3];
   uint8_t acc_risk_type_c;
} answer_margin_acc_item_t;



typedef struct answer_margin_ext_item
{
   series_t series;
   int64_t margin_req_u;
   int64_t market_value_q;
   account_t account;
   char currency_s [3];
   char filler_1_s;
} answer_margin_ext_item_t;



typedef struct answer_margin_mem_item
{
   char country_id_s [2];
   char ex_customer_s [5];
   char risk_currency_s [3];
   char cust_bank_id_s [12];
   char filler_2_s [2];
   int64_t risk_margin_q;
} answer_margin_mem_item_t;



typedef struct answer_margin_param_block_item
{
   char pur_id_s [15];
   char neg_time_adj_c;
   int64_t iter_accuracy_q;
   int32_t erosion_i;
   int32_t held_vol_max_i;
   int32_t writ_vol_min_i;
   int32_t held_val_max_i;
   int32_t writ_val_min_i;
   int32_t held_zero_limit_i;
   int32_t swap_lead_time_i;
   int32_t fix_rate_down_i;
   int32_t fix_rate_up_i;
   int32_t val_closed_risk_down_i;
   int32_t val_closed_risk_up_i;
   int32_t hrm_corr_i;
   int32_t pow_offset_days_i;
   int32_t points_reg_i;
   int32_t vol_steps_held_i;
   int32_t vol_steps_writ_i;
   int32_t vol_spread_held_i;
   int32_t vol_spread_writ_i;
   int32_t float_swap_steps_i;
   int32_t er_trd_days_in_year_i;
   int32_t sw_trd_days_in_year_i;
   int32_t sw_settl_days_i;
   int32_t marg_settl_days_i;
   int32_t iter_low_bound_i;
   int32_t iter_high_bound_i;
   int32_t iter_max_no_i;
   int32_t ulg_price_spread_i;
   int32_t bin_val_time_step_i;
   char vol_used_c;
   char opt_price_base_1_c;
   char opt_price_base_2_c;
   char corr_method_c;
   uint8_t margin_dividend_c;
   uint8_t margin_deliv_c;
   uint8_t margin_payment_c;
   uint8_t val_ivl_type_c;
   int32_t allwd_price_move_i;
   uint8_t val_ivl_base_c;
   uint8_t price_move_guard_c;
   uint8_t vol_interval_type_c;
   uint8_t base_offset_days_c;
   uint16_t offset_days_n;
   uint8_t real_time_price_use_c;
   uint8_t interest_rate_type_c;
   uint16_t day_count_n;
   uint8_t forw_margin_c;
   char reserved_1_s;
   char filler_40_s [40];
} answer_margin_param_block_item_t;



typedef struct answer_margin_pa_acc_item
{
   account_t account;
   char market_currency_s [3];
   char filler_1_s;
   int64_t market_margin_q;
   int64_t market_value_q;
   uint8_t prod_area_c;
   uint8_t acc_risk_type_c;
   char prod_area_text_s [10];
   char cust_bank_id_s [12];
} answer_margin_pa_acc_item_t;



typedef struct answer_margin_series_param_item
{
   series_t series;
   int32_t down_int_i;
   int32_t up_int_i;
   int32_t risk_free_rate_i;
   int32_t held_vol_down_i;
   int32_t held_vol_up_i;
   int32_t writ_vol_down_i;
   int32_t writ_vol_up_i;
   int32_t fixed_vol_i;
   int32_t held_for_adj_i;
   int32_t writ_for_adj_i;
   char pur_id_s [15];
   char win_id_s [15];
   char filler_2_s [2];
   int32_t bid_marg_vol_i;
   int32_t ask_marg_vol_i;
   int32_t dividend_yield_i;
} answer_margin_series_param_item_t;



typedef struct answer_margin_series_price_item
{
   series_t series;
   uint32_t bid_price_i;
   uint32_t ask_price_i;
   int32_t marg_price_i;
   int32_t last_paid_i;
   uint8_t bid_theo_c;
   uint8_t ask_theo_c;
   uint8_t last_theo_c;
   uint8_t marg_theo_c;
} answer_margin_series_price_item_t;



typedef struct answer_margin_simulation_item
{
   int64_t market_margin_q;
   int64_t risk_margin_q;
   char market_currency_s [3];
   char risk_currency_s [3];
   uint8_t sim_item_type_c;
   char filler_1_s;
   int64_t nbr_held_q;
   int64_t nbr_written_q;
   int64_t market_value_q;
   int64_t naked_margin_q;
   series_t series;
   uint32_t bid_price_i;
   uint32_t ask_price_i;
   int32_t marg_price_i;
   int32_t fixing_value_i;
   int32_t val_ivl_mid_i;
   int32_t val_ivl_low_i;
   int32_t val_ivl_high_i;
   uint16_t dec_in_price_n;
   char filler_2_s [2];
   char filler_8_s [8];
} answer_margin_simulation_item_t;



typedef struct answer_margin_ulg_price_item
{
   uint16_t commodity_n;
   char filler_2_s [2];
   uint32_t bid_price_i;
   uint32_t ask_price_i;
   int32_t marg_price_i;
   int32_t last_paid_i;
   uint8_t bid_theo_c;
   uint8_t ask_theo_c;
   uint8_t last_theo_c;
   uint8_t marg_theo_c;
} answer_margin_ulg_price_item_t;



typedef struct answer_margin_vector_item
{
   series_t series;
   uint32_t point_i;
   uint32_t spot_i;
   uint32_t held_low_i;
   uint32_t written_low_i;
   uint32_t held_middle_i;
   uint32_t written_middle_i;
   uint32_t held_high_i;
   uint32_t written_high_i;
   char created_date_s [8];
} answer_margin_vector_item_t;



typedef struct answer_margin_vector_alt_item
{
   series_t series;
   uint32_t point_i;
   uint32_t spot_i;
   int32_t held_low_risk_i;
   int32_t written_low_risk_i;
   int32_t held_middle_risk_i;
   int32_t written_middle_risk_i;
   int32_t held_high_risk_i;
   int32_t written_high_risk_i;
   int32_t marg_vec_ex1_i;
   int32_t marg_vec_ex2_i;
   uint16_t dec_in_price_n;
   uint16_t dec_in_vectors_n;
   char created_date_s [8];
} answer_margin_vector_alt_item_t;



typedef struct answer_market_item
{
   uint8_t country_c;
   uint8_t market_c;
   char name_s [32];
   char filler_2_s [2];
} answer_market_item_t;



typedef struct answer_market_da207_item
{
   da207_t da207;
} answer_market_da207_item_t;



typedef struct answer_member_int_item
{
   char country_id_s [2];
   char ex_customer_s [5];
   char name_s [32];
   char member_type_c;
   char clh_id_s [12];
   char address_1_s [32];
   char address_2_s [32];
   char address_3_s [32];
   char address_4_s [32];
   char address_5_s [32];
   char fax_s [16];
   char telephone_s [16];
   char contact_person_s [32];
   char sax_id_s [6];
   uint8_t default_sax_id_c;
   char filler_1_s;
} answer_member_int_item_t;



typedef struct answer_member_list_item
{
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_1_s;
} answer_member_list_item_t;



typedef struct answer_missing_deal_item
{
   struct
   {
       cl_trade_api_t cl_trade_api;
   } trd_part [2];
} answer_missing_deal_item_t;



typedef struct answer_missing_delivery_item
{
   cl_delivery_api_t cl_delivery_api;
} answer_missing_delivery_item_t;



typedef struct answer_missing_trade_item
{
   cl_trade_api_t cl_trade_api;
} answer_missing_trade_item_t;



typedef struct answer_mm_exposure_limit_ua103_item
{
   series_t series;
   uint32_t curtailment_trading_threshold_u;
   uint16_t curtailment_spread_factor_n;
   char filler_2_s [2];
   int32_t reserved_i;
} answer_mm_exposure_limit_ua103_item_t;



typedef struct answer_mm_parameters_ua101_item
{
   series_t series;
   mm_parameters_t mm_parameters;
   uint8_t expiration_group_c;
   uint8_t strike_price_group_c;
   char filler_2_s [2];
} answer_mm_parameters_ua101_item_t;



typedef struct answer_non_trading_days_item
{
   uint8_t country_c;
   uint8_t market_c;
   char date_non_trading_s [8];
   uint8_t type_non_trading_c;
   char filler_1_s;
} answer_non_trading_days_item_t;



typedef struct answer_ob_item
{
   quad_word order_number_u;
   uint16_t ob_position_n;
   uint8_t combo_mark_c;
   char filler_1_s;
   order_no_id_t order_no_id;
} answer_ob_item_t;



typedef struct answer_ob_id_item
{
   quad_word order_number_u;
   uint16_t ob_position_n;
   uint8_t combo_mark_c;
   char filler_1_s;
   order_t order;
} answer_ob_id_item_t;



typedef struct answer_ob_ma103_item
{
   series_t series;
   trading_code_t trading_code;
   quad_word order_number_u;
   int32_t premium_i;
   uint32_t quantity_u;
   uint32_t open_quantity_u;
   uint16_t isemm_trade_limit_absolute_n;
   uint16_t firm_trade_limit_absolute_n;
   uint16_t farmm_trade_limit_absolute_n;
   uint16_t time_validity_n;
   uint16_t order_category_n;
   uint8_t bid_or_ask_c;
   uint8_t lock_type_c;
   uint8_t order_type_c;
   uint8_t client_category_c;
   char filler_2_s [2];
} answer_ob_ma103_item_t;



typedef struct answer_ob_volumes_item
{
   series_t series;
   uint16_t block_n;
   uint16_t dec_in_strike_price_n;
   uint16_t dec_in_premium_n;
   char base_cur_s [3];
   char filler_3_s [3];
   struct
   {
       int32_t bid_premium_i;
   } items1 [5];
   struct
   {
       int32_t ask_premium_i;
   } items2 [5];
   struct
   {
       uint32_t bid_demand_u;
   } items3 [5];
   struct
   {
       uint32_t ask_demand_u;
   } items4 [5];
} answer_ob_volumes_item_t;



typedef struct answer_open_interest_item
{
   series_t series;
   uint64_t final_open_interest_q;
} answer_open_interest_item_t;



typedef struct answer_open_interest_ext_item
{
   series_t series;
   uint64_t gross_open_interest_q;
   uint64_t net_open_interest_q;
   uint64_t member_net_open_interest_q;
} answer_open_interest_ext_item_t;



typedef struct answer_opra_prices_ia111_item
{
   series_t series;
   int32_t best_bid_premium_i;
   int32_t best_ask_premium_i;
   int32_t best_bid_quantity_i;
   int32_t best_ask_quantity_i;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   uint32_t volume_u;
   uint32_t turnover_u;
   uint8_t country_ask_c;
   uint8_t country_bid_c;
   uint8_t country_last_c;
   uint8_t country_low_c;
   uint8_t country_high_c;
   uint8_t away_market_status_ask_c;
   uint8_t away_market_status_bid_c;
   uint8_t away_market_status_last_c;
   uint8_t away_market_status_low_c;
   uint8_t away_market_status_high_c;
   char filler_2_s [2];
} answer_opra_prices_ia111_item_t;



typedef struct answer_order_item
{
   quad_word order_number_u;
   uint16_t ob_position_n;
   uint8_t combo_mark_c;
   char filler_1_s;
   party_t party;
   order_t order;
} answer_order_item_t;



typedef struct answer_order_ma101_item
{
   series_t series;
   trading_code_t trading_code;
   quad_word order_number_u;
   int32_t premium_i;
   uint32_t quantity_u;
   uint16_t time_validity_n;
   uint16_t order_category_n;
   uint8_t client_category_c;
   uint8_t bid_or_ask_c;
   uint8_t order_type_c;
   uint8_t ignore_away_market_c;
   uint8_t lock_type_c;
   uint8_t open_close_req_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
} answer_order_ma101_item_t;



typedef struct answer_partition_item
{
   server_partition_t server_partition;
} answer_partition_item_t;



typedef struct answer_party_item
{
   party_no_id_t party_no_id;
} answer_party_item_t;



typedef struct answer_paynote_info_item
{
   series_t series;
   account_t account;
   char business_date_s [8];
   uint8_t event_type_c;
   uint8_t settle_class_c;
   char filler_2_s [2];
   int64_t amount_u;
   char currency_s [3];
   uint8_t pay_or_receive_c;
} answer_paynote_info_item_t;



typedef struct answer_physical_delivery_item
{
   char created_date_s [8];
   char as_of_date_s [8];
   char modified_date_s [8];
   confirmed_by_t confirmed_by;
   trading_code_t trading_code;
   int32_t event_type_i;
   int32_t event_origin_i;
   int32_t sequence_no_i;
   physical_series_t physical_series;
   series_t series;
   int64_t deliv_base_quantity_q;
   int32_t bond_quotation_i;
   uint16_t dec_in_bq_n;
   account_from_t account_from;
   account_to_t account_to;
   char csd_account_from_s [20];
   char csd_account_to_s [20];
   char settlement_date_s [8];
   int32_t class_no_i;
   char currency_s [3];
   uint8_t state_c;
} answer_physical_delivery_item_t;



typedef struct answer_pmm_parameters_ua102_item
{
   series_t series;
   mm_parameters_t mm_parameters;
   pmm_parameters_t pmm_parameters;
   uint8_t expiration_group_c;
   uint8_t strike_price_group_c;
   char filler_2_s [2];
} answer_pmm_parameters_ua102_item_t;



typedef struct answer_position_item
{
   series_t series;
   char modified_date_s [8];
   char modified_time_s [6];
   char filler_2_s [2];
   int64_t nbr_held_q;
   int64_t nbr_written_q;
   int64_t deny_exercise_q;
   account_t account;
   uint32_t quantity_cover_u;
} answer_position_item_t;



typedef struct answer_prel_settlement_item
{
   series_t series;
   int32_t settl_price_i;
   char hhmmss_s [6];
   char filler_2_s [2];
} answer_prel_settlement_item_t;



typedef struct answer_prices_ia101_item
{
   series_t series;
   uint32_t best_bid_size_u;
   uint32_t best_ask_size_u;
   uint32_t time_received_bid_u;
   uint32_t time_received_ask_u;
   uint32_t volume_u;
   uint32_t turnover_u;
   uint32_t open_balance_u;
   int32_t best_bid_premium_i;
   int32_t best_ask_premium_i;
   int32_t closing_price_i;
   int32_t opening_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   uint16_t fast_market_level_n;
   uint8_t series_status_c;
   uint8_t country_bid_c;
   uint8_t country_ask_c;
   uint8_t cust_on_ask_c;
   uint8_t cust_on_bid_c;
   char filler_1_s;
} answer_prices_ia101_item_t;



typedef struct answer_price_depth_ia102_item
{
   uint32_t volume_u;
   uint32_t time_received_u;
   int32_t premium_i;
   uint16_t commodity_n;
   uint16_t series_short_number_n;
   uint8_t bid_or_ask_c;
   uint8_t price_level_c;
   uint8_t price_depth_c;
   uint8_t country_c;
} answer_price_depth_ia102_item_t;



typedef struct answer_price_info_item
{
   series_t series;
   uint16_t block_n;
   uint16_t dec_in_strike_price_n;
   uint16_t dec_in_premium_n;
   char base_cur_s [3];
   char filler_3_s [3];
   int32_t closing_price_i;
   int32_t opening_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   uint32_t volume_u;
   uint32_t turnover_u;
   uint32_t open_balance_u;
} answer_price_info_item_t;



typedef struct answer_propagate_item
{
   account_t account;
   uint8_t prop_type_c;
   char filler_3_s [3];
} answer_propagate_item_t;



typedef struct answer_proxy_item
{
   quad_word order_number_u;
   uint32_t sequence_number_u;
   uint16_t ob_position_n;
   uint8_t combo_mark_c;
   char filler_1_s;
   party_t party;
   order_t order;
   uint16_t total_volume_n;
   uint16_t display_quantity_n;
} answer_proxy_item_t;



typedef struct answer_quote_ma104_item
{
   series_t series;
   trading_code_t trading_code;
   quad_word order_number_u;
   int32_t premium_i;
   uint32_t quantity_u;
   uint32_t open_quantity_u;
   uint16_t isemm_trade_limit_absolute_n;
   uint16_t firm_trade_limit_absolute_n;
   uint16_t farmm_trade_limit_absolute_n;
   uint16_t step_up_buffer_n;
   uint16_t number_of_tick_down_levels_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
} answer_quote_ma104_item_t;



typedef struct answer_realtime_ulg_price_item
{
   uint32_t bid_price_i;
   uint32_t ask_price_i;
   int32_t last_paid_i;
   uint16_t commodity_n;
   uint8_t bid_theo_c;
   uint8_t ask_theo_c;
   uint8_t last_theo_c;
   char filler_3_s [3];
} answer_realtime_ulg_price_item_t;



typedef struct answer_rectify_d_item
{
   orig_deal_part_t orig_deal_part;
   rectify_deal_part_t rectify_deal_part;
} answer_rectify_d_item_t;



typedef struct answer_rectify_d_cont_item
{
   series_t series;
   int32_t trade_number_i;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
   int32_t trade_quantity_i;
} answer_rectify_d_cont_item_t;



typedef struct answer_rectify_ext_cont_item
{
   account_t account;
   int32_t trade_quantity_i;
   uint8_t open_close_req_c;
   char customer_info_s [15];
} answer_rectify_ext_cont_item_t;



typedef struct answer_rectify_t_item
{
   char created_date_s [8];
   char created_time_s [6];
   char asof_date_s [8];
   char asof_time_s [6];
   trading_code_t trading_code;
   user_code_t user_code;
   series_t series;
   int32_t trade_number_i;
   int32_t rectify_trade_number_i;
   uint8_t state_c;
   char filler_3_s [3];
} answer_rectify_t_item_t;



typedef struct answer_report_item
{
   series_t series;
   int32_t info_type_i;
   char date_s [8];
   char country_id_s [2];
   char ex_customer_s [5];
   char name_s [32];
   char description_s [40];
   uint8_t ascii_bin_c;
} answer_report_item_t;



typedef struct answer_rotation_ma114_item
{
   series_t series;
   int32_t opening_price_i;
   uint32_t volume_u;
   uint32_t volume_left_market_orders_u;
   uint8_t series_status_c;
   uint8_t rotation_reason_c;
   char filler_2_s [2];
} answer_rotation_ma114_item_t;



typedef struct answer_series_item
{
   series_t series;
   upper_level_series_t upper_level_series;
   int32_t contract_size_i;
   char ins_id_s [32];
   char isin_code_s [12];
   uint8_t suspended_c;
   char date_last_trading_s [8];
   char time_last_trading_s [6];
   char settlement_date_s [8];
   char start_date_s [8];
   char end_date_s [8];
   char date_delivery_start_s [8];
   char date_delivery_stop_s [8];
   uint8_t series_status_c;
} answer_series_item_t;



typedef struct answer_series_bo_item
{
   series_t series;
   upper_level_series_t upper_level_series;
   int32_t contract_size_i;
   char ins_id_s [32];
   char isin_code_s [12];
   uint8_t stopped_by_issue_c;
   char isin_code_old_s [12];
   char date_notation_s [8];
   char date_last_trading_s [8];
   char time_last_trading_s [6];
   char date_delivery_start_s [8];
   char date_delivery_stop_s [8];
   uint8_t deliverable_c;
   uint8_t suspended_c;
   uint8_t series_status_c;
   uint8_t tm_template_c;
   uint8_t tm_series_c;
   char settlement_date_s [8];
   char start_date_s [8];
   char end_date_s [8];
} answer_series_bo_item_t;



typedef struct answer_series_da202_item
{
   da202_t da202;
} answer_series_da202_item_t;



typedef struct answer_series_da232_item
{
   series_t series;
   char ins_id_s [32];
   char isin_code_s [12];
   int32_t contract_size_i;
   uint16_t series_short_number_n;
   uint8_t suspended_c;
   char filler_1_s;
} answer_series_da232_item_t;



typedef struct answer_series_delivery_item
{
   series_t series;
   uint16_t items_n;
   char filler_2_s [2];
   struct
   {
       series_t series;
   } series_delivery [400];
} answer_series_delivery_item_t;



typedef struct answer_tm_deal_item
{
   uint8_t dc_deal_state_c;
   char filler_3_s [3];
   trading_code_t trading_code;
   int32_t deal_number_i;
   quad_word order_number_u;
   order_t order;
   party_t party;
   int32_t deal_price_i;
   uint16_t deal_quantity_n;
   char created_date_s [8];
   char created_time_s [6];
} answer_tm_deal_item_t;



typedef struct answer_tot_equil_prices_item
{
   series_t series;
   int32_t equilibrium_price_i;
} answer_tot_equil_prices_item_t;



typedef struct answer_tot_inactive_item
{
   quad_word order_number_u;
   uint32_t sequence_number_u;
   uint8_t combo_mark_c;
   char filler_3_s [3];
   party_t party;
   order_t order;
   uint16_t total_volume_n;
   uint16_t display_quantity_n;
} answer_tot_inactive_item_t;



typedef struct answer_tot_ob_item
{
   quad_word order_number_u;
   uint32_t sequence_number_u;
   uint16_t ob_position_n;
   uint8_t combo_mark_c;
   char filler_1_s;
   order_no_id_t order_no_id;
} answer_tot_ob_item_t;



typedef struct answer_tot_ob_id_item
{
   quad_word order_number_u;
   uint32_t sequence_number_u;
   uint16_t ob_position_n;
   uint8_t combo_mark_c;
   char filler_1_s;
   order_t order;
} answer_tot_ob_id_item_t;



typedef struct answer_tot_order_item
{
   quad_word order_number_u;
   uint32_t sequence_number_u;
   uint16_t ob_position_n;
   uint8_t combo_mark_c;
   char filler_1_s;
   party_t party;
   order_t order;
   uint16_t total_volume_n;
   uint16_t display_quantity_n;
} answer_tot_order_item_t;



typedef struct answer_tot_party_item
{
   party_no_id_t party_no_id;
} answer_tot_party_item_t;



typedef struct answer_tot_volume_prices_item
{
   series_t series;
   uint16_t block_n;
   uint8_t premium_levels_c;
   char trend_indicator_c;
   struct
   {
       int32_t bid_premium_i;
       int32_t ask_premium_i;
       uint32_t bid_demand_u;
       uint32_t ask_demand_u;
   } ob_item [5];
   int32_t closing_price_i;
   int32_t opening_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   uint32_t volume_u;
   uint32_t turnover_u;
   uint32_t open_balance_u;
   uint32_t number_of_deals_u;
   uint32_t sequence_number_u;
   uint32_t rem_quantity_bid_u;
   uint32_t rem_quantity_ask_u;
   quad_word order_number_bid_u;
   quad_word order_number_ask_u;
} answer_tot_volume_prices_item_t;



typedef struct answer_tot_volume_prices_bbo_item
{
   series_t series;
   uint16_t block_n;
   uint8_t premium_levels_c;
   char trend_indicator_c;
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   uint32_t bid_demand_u;
   uint32_t ask_demand_u;
   int32_t closing_price_i;
   int32_t opening_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   uint32_t volume_u;
   uint32_t turnover_u;
   uint32_t open_balance_u;
} answer_tot_volume_prices_bbo_item_t;



typedef struct answer_trade_item
{
   cl_trade_api_t cl_trade_api;
} answer_trade_item_t;



typedef struct answer_trade_deriv_history_item
{
   char date_trading_s [8];
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   int32_t opening_price_i;
   int32_t last_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   uint32_t turnover_u;
   uint32_t open_balance_u;
   int32_t bid_volatility_i;
   int32_t ask_volatility_i;
} answer_trade_deriv_history_item_t;



typedef struct answer_trade_int_item
{
   cl_trade_api_t cl_trade_api;
} answer_trade_int_item_t;



typedef struct answer_trade_statistics_item
{
   series_t series;
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   int32_t opening_price_i;
   int32_t settle_price_i;
   int32_t last_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   uint32_t volume_today_i;
   uint32_t volume_yesterday_i;
   int64_t turnaround_yesterday_u;
   int64_t turnaround_today_u;
   uint32_t open_balance_u;
   int32_t volatility_i;
   int32_t underlying_price_i;
   uint8_t bid_theo_c;
   uint8_t ask_theo_c;
   char filler_2_s [2];
} answer_trade_statistics_item_t;



typedef struct answer_trade_uv_history_item
{
   char date_trading_s [8];
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   int32_t opening_price_i;
   int32_t last_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   uint32_t turnover_u;
   uint32_t open_balance_u;
   int32_t reserved_i;
} answer_trade_uv_history_item_t;



typedef struct answer_trading_state_item
{
   char state_name_s [20];
   uint16_t state_number_n;
   uint8_t continues_matching_c;
   uint8_t trading_end_c;
   uint8_t price_quotation_required_c;
   uint8_t market_orders_allowed_c;
   uint8_t fill_or_kill_allowed_c;
   uint8_t fill_and_kill_allowed_c;
   uint8_t edited_ob_changes_avail_c;
   uint8_t ob_changes_avail_c;
   uint8_t external_full_depth_c;
   uint8_t internal_full_depth_c;
   uint8_t end_of_clearing_day_c;
   char filler_3_s [3];
} answer_trading_state_item_t;



typedef struct answer_trading_state_da245_item
{
   char state_name_s [20];
   uint16_t state_number_n;
   uint16_t fast_market_level_n;
   uint8_t type_of_matching_c;
   uint8_t trading_end_c;
   uint8_t price_quotation_required_c;
   uint8_t market_orders_allowed_c;
   uint8_t fill_or_kill_allowed_c;
   uint8_t fill_and_kill_allowed_c;
   uint8_t edited_ob_changes_avail_c;
   uint8_t ob_changes_avail_c;
} answer_trading_state_da245_item_t;



typedef struct answer_underlying_item
{
   uint16_t commodity_n;
   char com_id_s [6];
   char isin_code_s [12];
   uint16_t dec_in_price_n;
   char date_release_s [8];
   char date_termination_s [8];
   char date_dated_s [8];
   char name_s [32];
   char base_cur_s [3];
   uint8_t deliverable_c;
   uint16_t coupon_frequency_n;
   int64_t nominal_value_q;
   uint16_t day_count_n;
   uint16_t days_in_interest_year_n;
   uint32_t coupon_interest_i;
   uint16_t coupon_settlement_days_n;
   uint8_t underlying_type_c;
   uint8_t price_unit_c;
   uint16_t dec_in_nominal_n;
   uint8_t fixed_income_type_c;
   char filler_3_s [3];
   uint16_t items_n;
   struct
   {
       char date_coupdiv_s [8];
       uint32_t dividend_i;
   } coupon [80];
} answer_underlying_item_t;



typedef struct answer_underlying_adjustment_item
{
   uint16_t adjust_ident_n;
   uint16_t commodity_n;
   char date_adjust_s [8];
   char date_conversion_s [8];
   uint8_t deal_price_modifier_c;
   uint8_t contract_size_modifier_c;
   uint8_t strike_price_modifier_c;
   uint8_t contracts_modifier_c;
   uint8_t und_price_modifier_c;
   uint8_t so_strike_price_modifier_c;
   uint8_t so_contract_size_modifier_c;
   uint8_t so_deal_price_modifier_c;
   int32_t deal_price_mod_factor_i;
   int32_t contr_size_mod_factor_i;
   int32_t strike_price_mod_factor_i;
   int32_t contracts_mod_factor_i;
   int32_t und_price_mod_factor_i;
   int32_t so_strike_price_mod_factor_i;
   int32_t so_contr_size_mod_factor_i;
   int32_t so_deal_price_mod_factor_i;
   uint16_t new_commodity_n;
   uint16_t so_commodity_n;
   uint8_t country_c;
   uint8_t market_c;
   uint8_t so_country_c;
   uint8_t so_market_c;
   uint8_t adjusted_c;
   uint8_t spinoff_c;
   uint16_t items_n;
   struct
   {
       series_t series;
       int32_t contract_share_i;
   } delivery_change [20];
} answer_underlying_adjustment_item_t;



typedef struct answer_underlying_da204_item
{
   da204_t da204;
} answer_underlying_da204_item_t;



typedef struct answer_underlying_indices_item
{
   uint16_t commodity_n;
   char filler_2_s [2];
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   int32_t closing_price_i;
   int32_t opening_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   int32_t ref_price_i;
   uint32_t turnover_u;
   char date_s [8];
   char ext_time_s [6];
   char reserved_2_s [2];
} answer_underlying_indices_item_t;



typedef struct answer_underlying_prices_ia121_item
{
   int32_t best_bid_premium_i;
   int32_t best_ask_premium_i;
   int32_t closing_price_i;
   int32_t opening_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   uint32_t volume_u;
   uint32_t turnover_u;
   uint16_t commodity_n;
   uint8_t country_c;
   uint8_t nasdaq_upc_indicator_c;
   uint8_t nasdaq_short_sale_bid_tick_c;
   uint8_t originator_id_c;
   char filler_2_s [2];
} answer_underlying_prices_ia121_item_t;



typedef struct answer_underlying_status_us_item
{
   uint16_t commodity_n;
   uint8_t underlying_trading_status_c;
   char filler_1_s;
} answer_underlying_status_us_item_t;



typedef struct answer_user_type_info_item
{
   transaction_type_t transaction_type;
   uint8_t trans_or_bdx_c;
   char filler_3_s [3];
} answer_user_type_info_item_t;



typedef struct answer_uv_price_item
{
   uint16_t commodity_n;
   char filler_2_s [2];
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   int32_t closing_price_i;
   int32_t opening_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   uint32_t turnover_u;
   char date_s [8];
   char ext_time_s [6];
   char reserved_2_s [2];
} answer_uv_price_item_t;



typedef struct answer_volume_prices_item
{
   series_t series;
   uint16_t block_n;
   uint8_t premium_levels_c;
   char trend_indicator_c;
   struct
   {
       int32_t bid_premium_i;
   } items1 [5];
   struct
   {
       int32_t ask_premium_i;
   } items2 [5];
   struct
   {
       uint32_t bid_demand_u;
   } items3 [5];
   struct
   {
       uint32_t ask_demand_u;
   } items4 [5];
   int32_t closing_price_i;
   int32_t opening_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   uint32_t volume_u;
   uint32_t turnover_u;
   uint32_t open_balance_u;
} answer_volume_prices_item_t;



typedef struct answer_vol_diss_step_da243_item
{
   series_t series;
   uint16_t items_n;
   char filler_2_s [2];
   struct
   {
       uint32_t volume_chg_u;
       uint32_t upper_volume_u;
   } vol_step [20];
} answer_vol_diss_step_da243_item_t;



typedef struct ans_pmm_price_imp_quote_ma107_item
{
   series_t series;
   quad_word order_number_u;
   uint32_t volume_limit_u;
   uint16_t volume_multiplier_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
} ans_pmm_price_imp_quote_ma107_item_t;



typedef struct away_market_flag_update_us_item
{
   uint16_t commodity_n;
   uint8_t country_c;
   uint8_t ignore_away_market_c;
} away_market_flag_update_us_item_t;



typedef struct changes
{
   uint32_t sequence_number_u;
   uint16_t ob_position_n;
   int16_t quantity_difference_w;
   uint8_t ob_command_c;
   uint8_t change_reason_c;
   uint8_t combo_mark_c;
   char filler_1_s;
} changes_t;



typedef struct bb_ob_changes_id_item
{
   bb_order_no_id_t bb_order_no_id;
} bb_ob_changes_id_item_t;



typedef struct bb_ob_changes_no_id_item
{
   bb_order_no_id_t bb_order_no_id;
} bb_ob_changes_no_id_item_t;



typedef struct best_bid_offer_bo101_item
{
   series_t series;
   int32_t best_bid_premium_i;
   int32_t best_ask_premium_i;
   uint32_t best_bid_size_u;
   uint32_t best_ask_size_u;
   uint8_t cust_on_ask_c;
   uint8_t cust_on_bid_c;
   char filler_2_s [2];
} best_bid_offer_bo101_item_t;



typedef struct best_bid_offer_bo119_item
{
   int32_t best_bid_premium_i;
   int32_t best_ask_premium_i;
   uint32_t best_bid_size_u;
   uint32_t best_ask_size_u;
   uint16_t commodity_n;
   uint16_t series_short_number_n;
} best_bid_offer_bo119_item_t;



typedef struct best_bid_offer_extended_bo102_item
{
   series_t series;
   int32_t best_bid_premium_i;
   int32_t best_ask_premium_i;
   uint32_t best_bid_size_u;
   uint32_t best_ask_size_u;
   uint32_t bid_market_order_size_u;
   uint32_t ask_market_order_size_u;
   uint16_t mm_number_n;
   uint8_t cust_on_ask_c;
   uint8_t cust_on_bid_c;
} best_bid_offer_extended_bo102_item_t;



typedef struct block_order_response_item
{
   uint32_t trans_ack_u;
   uint8_t item_number_c;
   char filler_3_s [3];
} block_order_response_item_t;



typedef struct block_order_trans_item
{
   series_t series;
   order_var_t order_var;
   uint16_t total_volume_n;
   char filler_2_s [2];
} block_order_trans_item_t;



typedef struct block_order_trans_p_item
{
   series_t series;
   order_var_t order_var;
   uint16_t total_volume_n;
   char filler_2_s [2];
} block_order_trans_p_item_t;



typedef struct block_price_trans_item
{
   series_t series;
   quad_word order_number_bid_u;
   quad_word order_number_ask_u;
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   int16_t bid_quantity_w;
   int16_t ask_quantity_w;
   int16_t bid_total_volume_w;
   int16_t ask_total_volume_w;
   uint16_t block_n;
   uint16_t time_validity_n;
   uint8_t order_type_c;
   char ex_client_s [10];
   uint8_t delta_quantity_c;
} block_price_trans_item_t;



typedef struct block_price_trans_p_item
{
   series_t series;
   quad_word order_number_bid_u;
   quad_word order_number_ask_u;
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   int16_t bid_quantity_w;
   int16_t ask_quantity_w;
   int16_t bid_total_volume_w;
   int16_t ask_total_volume_w;
   uint16_t block_n;
   uint16_t time_validity_n;
   uint8_t order_type_c;
   char ex_client_s [10];
   uint8_t delta_quantity_c;
} block_price_trans_p_item_t;



typedef struct ced_answer_daily_trade_stat_item
{
   ced_daily_trade_statistics_t ced_daily_trade_statistics;
} ced_answer_daily_trade_stat_item_t;



typedef struct clearing_message_item
{
   char text_line_s [80];
} clearing_message_item_t;



typedef struct cl_rectify_trade_item
{
   account_t account;
   int32_t trade_quantity_i;
   uint8_t open_close_req_c;
   char customer_info_s [15];
} cl_rectify_trade_item_t;



typedef struct cl_reregistration_item
{
   quad_word order_number_u;
   int32_t deal_price_i;
   uint16_t deal_quantity_n;
   char ex_client_s [10];
   uint8_t open_close_req_c;
   char customer_info_s [15];
} cl_reregistration_item_t;



typedef struct cl_reregistration_bo_item
{
   int32_t trade_number_i;
   account_t account;
   char reserved_1_c;
   char reserved_2_s [2];
   char filler_1_s;
   uint16_t deal_quantity_n;
   uint8_t open_close_req_c;
   char customer_info_s [15];
   char filler_2_s [2];
} cl_reregistration_bo_item_t;



typedef struct combo_ob_changes_bo131_item
{
   series_t series;
   uint16_t multiplier_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
} combo_ob_changes_bo131_item_t;



typedef struct combo_ob_member_bo132_item
{
   series_t series;
   uint16_t multiplier_n;
   uint8_t bid_or_ask_c;
   uint8_t open_close_req_c;
} combo_ob_member_bo132_item_t;



typedef struct combo_order_trans_mo131_item
{
   series_t series;
   uint16_t multiplier_n;
   uint8_t bid_or_ask_c;
   uint8_t open_close_req_c;
} combo_order_trans_mo131_item_t;



typedef struct da5
{
   combo_series_t combo_series;
   char cbs_id_s [32];
   uint8_t items_c;
   char filler_3_s [3];
   struct
   {
       series_t series;
       uint16_t ratio_n;
       char op_if_buy_c;
       char op_if_sell_c;
   } item [4];
} da5_t;



typedef struct confirm_give_up_request_item
{
   account_t account;
   int32_t trade_quantity_i;
   uint8_t open_close_req_c;
   char customer_info_s [15];
} confirm_give_up_request_item_t;



typedef struct originating_side
{
   uint32_t quantity_u;
   uint8_t bid_or_ask_c;
   char filler_1_s;
   uint16_t time_validity_n;
   uint8_t client_category_c;
   uint8_t open_close_req_c;
   char ex_client_s [10];
   give_up_member_t give_up_member;
   char broker_id_s [5];
   char free_text_s [15];
} originating_side_t;



typedef struct counter_party_side
{
   uint8_t broker_percentage_c;
   uint8_t bid_or_ask_c;
   uint16_t time_validity_n;
   uint8_t client_category_c;
   uint8_t open_close_req_c;
   char ex_client_s [10];
   give_up_member_t give_up_member;
   char broker_id_s [5];
   char free_text_s [15];
} counter_party_side_t;



typedef struct dc_deal_user_item
{
   quad_word order_number_u;
   uint16_t deal_quantity_n;
   uint16_t block_n;
   uint8_t bid_or_ask_c;
   uint8_t deal_source_c;
} dc_deal_user_item_t;



typedef struct deal_user_item
{
   quad_word order_number_u;
   uint16_t deal_quantity_n;
   uint16_t rem_quantity_n;
   uint8_t bid_or_ask_c;
   uint8_t deal_source_c;
   uint16_t block_n;
} deal_user_item_t;



typedef struct deal_user_bd111_item
{
   quad_word order_number_u;
   uint32_t quantity_u;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} deal_user_bd111_item_t;



typedef struct whose
{
   trading_code_t trading_code;
   char ex_client_s [10];
   char filler_2_s [2];
} whose_t;



typedef struct de_series_status_bi104_item
{
   series_t series;
   int32_t reserved_i;
   uint8_t series_status_c;
   uint8_t country_c;
   char filler_2_s [2];
} de_series_status_bi104_item_t;



typedef struct directed_deal_item
{
   cl_trade_api_t cl_trade_api;
   uint32_t open_balance_u;
   int64_t nbr_held_q;
   int64_t nbr_written_q;
} directed_deal_item_t;



typedef struct dsg_exchg_list_status_info_item
{
   series_t series;
   uint16_t designated_list_n;
   char filler_2_s [2];
} dsg_exchg_list_status_info_item_t;



typedef struct equil_price_update_item
{
   series_t series;
   int32_t equilibrium_price_i;
} equil_price_update_item_t;



typedef struct execute_combo_trans_mo136_item
{
   series_t series;
   uint8_t open_close_req_c;
   char filler_2_s [2];
   char filler_1_s;
} execute_combo_trans_mo136_item_t;



typedef struct execution_report_list_mo122_item
{
   series_t series;
   quad_word order_number_u;
   int32_t sequence_number_i;
   int32_t deal_price_i;
   uint32_t volume_u;
   uint16_t hedge_order_id_n;
   uint8_t bid_or_ask_c;
   uint8_t end_of_report_c;
   uint8_t hedge_status_c;
   char execution_reference_s [30];
   char filler_1_1_s;
} execution_report_list_mo122_item_t;



typedef struct extended_underlying_info_item
{
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   uint32_t bid_demand_u;
   uint32_t ask_demand_u;
} extended_underlying_info_item_t;



typedef struct ext_answer_missing_trade_proxy_item
{
   trading_code_t trading_code;
   cl_trades_t cl_trades;
   int32_t ext_seq_nbr_i;
   int32_t ext_status_i;
   int32_t ext_state_i;
   account_t account;
   pos_account_t pos_account;
   int32_t reserved_1_i;
   int32_t reserved_2_i;
} ext_answer_missing_trade_proxy_item_t;



typedef struct ext_quote_reject_reason_bo125_item
{
   int32_t reason_quote_rejected_i;
   uint8_t item_number_c;
   char filler_3_s [3];
} ext_quote_reject_reason_bo125_item_t;



typedef struct general_message_item
{
   char text_line_s [80];
} general_message_item_t;



typedef struct hedge_price_update_item
{
   series_t series;
   int32_t hedge_price_i;
} hedge_price_update_item_t;



typedef struct instrument_status_info_item
{
   series_t series;
   uint16_t minutes_to_state_change_n;
   uint16_t state_number_n;
   char warning_msg_s [80];
} instrument_status_info_item_t;



typedef struct da10_da20
{
   series_t series;
   upper_level_series_t upper_level_series;
   int32_t price_quot_factor_i;
   int32_t contract_size_i;
   int32_t exerc_limit_i;
   int32_t redemption_value_i;
   uint16_t derivate_level_n;
   uint16_t dec_in_strike_price_n;
   uint16_t dec_in_contr_size_n;
   uint16_t rnt_id_n;
   uint16_t dec_in_premium_n;
   uint16_t items_n;
   struct
   {
       tick_size_t tick_size;
   } item [12];
   uint16_t items_block_n;
   char filler_2_s [2];
   struct
   {
       uint16_t block_n;
   } block_size [2];
   char base_cur_s [3];
   uint8_t traded_c;
   uint8_t exerc_limit_unit_c;
   char inc_id_s [10];
   char name_s [32];
   char is_fractions_c;
   uint8_t price_format_c;
   uint8_t strike_price_format_c;
   uint8_t cabinet_format_c;
   uint8_t price_unit_premium_c;
   uint8_t price_unit_strike_c;
   char filler_3_s [3];
} da10_da20_t;



typedef struct long_position_adj_item
{
   account_t account;
   series_t series;
   int32_t long_adjustment_i;
} long_position_adj_item_t;



typedef struct sts_gen
{
   mar_gen_t mar_gen;
   uint8_t on_off_c;
   char filler_3_s [3];
} sts_gen_t;



typedef struct message_bi101_item
{
   char free_text_80_s [80];
} message_bi101_item_t;



typedef struct modify_commission_item
{
   series_t series;
   party_t party;
   char account_id_s [10];
   char customer_info_s [15];
   char created_date_s [8];
   char created_time_s [6];
   char user_code_s [12];
   char filler_1_s;
   int32_t commission_i;
} modify_commission_item_t;



typedef struct mp_deal_item
{
   mp_trade_t mp_trade;
   int32_t deal_number_i;
} mp_deal_item_t;



typedef struct multiple_order_trans_mo124_item
{
   series_t series;
   int32_t premium_i;
   uint32_t quantity_u;
   uint16_t time_validity_n;
   uint8_t bid_or_ask_c;
   uint8_t order_type_c;
   uint8_t ignore_away_market_c;
   uint8_t client_category_c;
   uint8_t open_close_req_c;
   char ex_client_s [10];
   char free_text_s [15];
   char broker_id_s [5];
   char filler_3_s [3];
} multiple_order_trans_mo124_item_t;



typedef struct ob_levels_price_volumes_item
{
   int32_t premium_i;
   uint32_t demand_u;
} ob_levels_price_volumes_item_t;



typedef struct ob_volumes_bbo_item
{
   series_t series;
   uint16_t block_n;
   uint8_t premium_levels_c;
   char filler_1_s;
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   uint32_t bid_demand_u;
   uint32_t ask_demand_u;
} ob_volumes_bbo_item_t;



typedef struct opra_all_best_bid_offer_bo104_item
{
   series_t series;
   int32_t best_bid_premium_i;
   int32_t best_ask_premium_i;
   int32_t best_bid_quantity_i;
   int32_t best_ask_quantity_i;
   int32_t reserved_1_i;
   int32_t reserved_2_i;
   uint8_t country_c;
   uint8_t away_market_status_c;
   char filler_2_s [2];
} opra_all_best_bid_offer_bo104_item_t;



typedef struct opra_best_bid_offer_bo103_item
{
   series_t series;
   int32_t best_bid_premium_i;
   int32_t best_ask_premium_i;
   int32_t best_bid_quantity_i;
   int32_t best_ask_quantity_i;
   int32_t reserved_1_i;
   int32_t reserved_2_i;
   uint8_t country_bid_c;
   uint8_t country_ask_c;
   uint8_t away_market_status_bid_c;
   uint8_t away_market_status_ask_c;
} opra_best_bid_offer_bo103_item_t;



typedef struct orders_to_be_hedged_bo118_item
{
   uint32_t quantity_u;
   int32_t premium_i;
   uint16_t hedge_order_id_n;
   uint8_t order_type_c;
   char filler_1_s;
} orders_to_be_hedged_bo118_item_t;



typedef struct pmm_price_imp_quote_mo123_item
{
   series_t series;
   uint32_t volume_limit_u;
   uint16_t volume_multiplier_n;
   uint8_t bid_or_ask_c;
   char filler_1_1_s;
} pmm_price_imp_quote_mo123_item_t;



typedef struct price_depth_info_bo117_item
{
   uint32_t volume_u;
   int32_t premium_i;
   uint16_t commodity_n;
   uint16_t series_short_number_n;
   uint8_t bid_or_ask_c;
   uint8_t price_level_c;
   uint8_t price_depth_c;
   char filler_1_s;
} price_depth_info_bo117_item_t;



typedef struct search_series
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} search_series_t;



typedef struct filter_series
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} filter_series_t;



typedef struct query_margin_simulation_item
{
   uint8_t item_type_c;
   char filler_3_s [3];
   series_t series;
   int64_t sim_qty_q;
   int32_t trade_price_sim_i;
   int32_t reserved_i;
   char closing_date_s [8];
   char date_settlement_s [8];
   char reserved_8_s [8];
} query_margin_simulation_item_t;



typedef struct quote_trans_mo106_item
{
   series_t series;
   int32_t premium_i;
   uint32_t quantity_u;
   uint32_t open_quantity_u;
   uint16_t step_up_buffer_n;
   uint8_t bid_or_ask_c;
   char filler_1_1_s;
} quote_trans_mo106_item_t;



typedef struct other_series
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} other_series_t;



typedef struct rectify_deal_item
{
   int32_t trade_number_i;
   int32_t trade_quantity_i;
   uint8_t bid_or_ask_c;
   char reserved_1_c;
   char reserved_2_s [2];
} rectify_deal_item_t;



typedef struct rectify_trade_item
{
   account_t account;
   int32_t trade_quantity_i;
   uint8_t open_close_req_c;
   char customer_info_s [15];
} rectify_trade_item_t;



typedef struct rectify_trade_ise_item
{
   int32_t trade_quantity_i;
   uint8_t open_close_req_c;
   char account_type_c;
   char customer_info_s [15];
   char sub_account_id_s [5];
   char cmta_member_s [5];
   char filler_1_s;
} rectify_trade_ise_item_t;



typedef struct reg_physical_delivery_item
{
   physical_series_t physical_series;
   int64_t deliv_base_quantity_q;
} reg_physical_delivery_item_t;



typedef struct rotation_request_mo113_mq114_item
{
   series_t series;
   uint8_t ignore_away_market_c;
   char filler_3_s [3];
} rotation_request_mo113_mq114_item_t;



typedef struct da9
{
   series_t series;
   upper_level_series_t upper_level_series;
   int32_t contract_size_i;
   char ins_id_s [32];
   char isin_code_s [12];
   uint8_t stopped_by_issue_c;
   char isin_code_old_s [12];
   char date_notation_s [8];
   char date_last_trading_s [8];
   char time_last_trading_s [6];
   char date_delivery_start_s [8];
   char date_delivery_stop_s [8];
   uint8_t deliverable_c;
   uint8_t suspended_c;
   uint8_t series_status_c;
   uint8_t tm_template_c;
   uint8_t tm_series_c;
   char settlement_date_s [8];
   char start_date_s [8];
   char end_date_s [8];
} da9_t;



typedef struct series_status_change_bi103_item
{
   series_t series;
   int32_t opening_price_i;
   uint32_t volume_u;
   uint16_t fast_market_level_n;
   uint8_t series_status_c;
   char filler_1_s;
} series_status_change_bi103_item_t;



typedef struct settle_price_update_item
{
   series_t series;
   int32_t settle_price_i;
} settle_price_update_item_t;



typedef struct set_mm_exposure_limit_ui103_item
{
   series_t series;
   uint32_t curtailment_trading_threshold_u;
   uint16_t curtailment_spread_factor_n;
   char filler_2_s [2];
   int32_t reserved_i;
} set_mm_exposure_limit_ui103_item_t;



typedef struct set_mm_parameters_ui101_item
{
   series_t series;
   mm_parameters_t mm_parameters;
   uint8_t expiration_group_c;
   uint8_t strike_price_group_c;
   char filler_2_s [2];
} set_mm_parameters_ui101_item_t;



typedef struct set_pmm_parameters_ui102_item
{
   series_t series;
   mm_parameters_t mm_parameters;
   pmm_parameters_t pmm_parameters;
   uint8_t expiration_group_c;
   uint8_t strike_price_group_c;
   char filler_2_s [2];
} set_pmm_parameters_ui102_item_t;



typedef struct stop_series
{
   uint8_t country_c;
   uint8_t market_c;
   uint8_t instrument_group_c;
   uint8_t modifier_c;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} stop_series_t;



typedef struct da4_da19
{
   uint16_t commodity_n;
   char com_id_s [6];
   char isin_code_s [12];
   uint16_t dec_in_price_n;
   char date_release_s [8];
   char date_termination_s [8];
   char date_dated_s [8];
   char name_s [32];
   char base_cur_s [3];
   uint8_t deliverable_c;
   uint16_t coupon_frequency_n;
   int64_t nominal_value_q;
   uint16_t day_count_n;
   uint16_t days_in_interest_year_n;
   uint32_t coupon_interest_i;
   uint16_t coupon_settlement_days_n;
   uint8_t underlying_type_c;
   uint8_t price_unit_c;
   uint16_t dec_in_nominal_n;
   uint8_t fixed_income_type_c;
   char filler_3_s [3];
   uint16_t items_n;
   struct
   {
       char date_coupdiv_s [8];
       uint32_t dividend_i;
   } coupon [80];
} da4_da19_t;


/*  TRANSACTION TYPE = CA42, CA2090  */

typedef struct account_status
{
   transaction_type_t transaction_type;
   account_t account;
   uint16_t items_n;
   char filler_2_s [2];
   account_status_item_t item [201];
} account_status_t;


/* TRANSACTION TYPE = CA36  */

typedef struct answer_average_price_trade
{
   transaction_type_t transaction_type;
   series_t series;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_average_price_trade_item_t item [1000];
} answer_average_price_trade_t;


/* TRANSACTION TYPE = CD32, CD288   */

typedef struct average_price_trade
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t items_n;
   char filler_2_s [2];
   average_price_trade_item_t item [1000];
} average_price_trade_t;


/*  TRANSACTION TYPE = CA33 */

typedef struct ced_answer_assignment_proxy
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   ced_answer_assignment_proxy_item_t item [400];
} ced_answer_assignment_proxy_t;


/*  TRANSACTION TYPE = CA35 */

typedef struct ced_answer_class_file_info
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   char yyyymmdd_s [8];
   ced_answer_class_file_info_item_t item [400];
} ced_answer_class_file_info_t;


/*  TRANSACTION TYPE = CA32 */

typedef struct ced_answer_missing_exbyex_proxy
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   char filler_2_s [2];
   ced_answer_missing_exbyex_proxy_item_t item [400];
} ced_answer_missing_exbyex_proxy_t;


/*  TRANSACTION TYPE = CA34 */

typedef struct ced_answer_theoretical_values
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   char yyyymmdd_s [8];
   ced_answer_theoretical_values_item_t item [400];
} ced_answer_theoretical_values_t;


/*  BROADCAST TYPE = BD12 */

typedef struct ced_exercise_info_brd
{
   broadcast_type_t broadcast_type;
   ced_exercise_info_t ced_exercise_info;
} ced_exercise_info_brd_t;


/*  TRANSACTION TYPE = CC20, CC21 */

typedef struct ced_exercise_rqst
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t quantity_i;
   char account_id_s [10];
   char filler_2_s [2];
   int32_t rqst_type_i;
   char customer_info_s [15];
   char filler_1_s;
} ced_exercise_rqst_t;


/*  BROADCAST TYPE = BD13  */

typedef struct ced_intraday_margin_calls_brd
{
   broadcast_type_t broadcast_type;
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_1_s;
   char yyyymmdd_s [8];
   char hhmmss_s [6];
   char text_s [800];
   char filler_2_s [2];
} ced_intraday_margin_calls_brd_t;


/*  TRANSACTION TYPE = CQ33 */

typedef struct ced_query_assignment_proxy
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char yyyymmdd_s [8];
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_3_s [3];
} ced_query_assignment_proxy_t;


/*  TRANSACTION TYPE = CQ35 */

typedef struct ced_query_class_file_info
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char yyyymmdd_s [8];
   char filler_2_s [2];
} ced_query_class_file_info_t;


/*  TRANSACTION TYPE = CQ32 */

typedef struct ced_query_missing_exbyex_proxy
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t sequence_first_i;
   int32_t sequence_last_i;
   char yyyymmdd_s [8];
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_1_s;
} ced_query_missing_exbyex_proxy_t;


/*  TRANSACTION TYPE = CQ34 */

typedef struct ced_query_theoretical_values
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char yyyymmdd_s [8];
   char filler_2_s [2];
} ced_query_theoretical_values_t;


/* TRANSACTION TYPE = CQ36, CQ292   */

typedef struct query_average_price_trade
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
   int32_t trade_number_i;
} query_average_price_trade_t;


/*  TRANSACTION TYPE = CQ65, CQ321, CQ2113  */

typedef struct query_pos_level
{
   transaction_type_t transaction_type;
   series_t series;
   account_t account;
   char series_id_s [32];
   int32_t summary_i;
   uint16_t segment_number_n;
   char filler_2_s [2];
   char date_s [8];
   char account_type_s [12];
   int32_t level_type_i;
} query_pos_level_t;


/*  TRANSACTION TYPE = RQ44, RQ300  */

typedef struct query_realtime_ulg_price
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   char filler_2_s [2];
} query_realtime_ulg_price_t;


/*  TRANSACTION TYPE = CC42, CC298, CC2090  */

typedef struct register_account
{
   transaction_type_t transaction_type;
   series_t series;
   account_t account;
   store_account_t store_account;
} register_account_t;


/*   TRANSACTION TYPE = DA62 */

typedef struct account_gen
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   char filler_2_s [2];
   account_gen_item_t item [100];
} account_gen_t;


/*   TRANSACTION TYPE = DC3  */

typedef struct add_tm_combo
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t no_of_legs_n;
   char filler_2_s [2];
   add_tm_combo_item_t item [4];
} add_tm_combo_t;


/*  TRANSACTION TYPE = MO133  */

typedef struct alter_combo_trans_mo133
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   uint8_t order_type_c;
   uint8_t interest_order_c;
   uint8_t client_category_c;
   char ex_client_s [10];
   uint16_t reserved_4_n;
   int32_t ul_price_low_i;
   int32_t ul_price_high_i;
   int32_t premium_i;
   quad_word order_number_u;
   uint32_t quantity_u;
   uint16_t time_validity_n;
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char ul_price_method_c;
   char filler_1_s;
   char reserved_32_s [32];
   uint8_t items_c;
   char filler_3_3_s [3];
   alter_combo_trans_mo133_item_t item [10];
} alter_combo_trans_mo133_t;


/*   TRANSACTION TYPE = MO3, MO43  */

typedef struct alter_trans
{
   transaction_type_t transaction_type;
   series_t series;
   quad_word order_number_u;
   order_var_t order_var;
} alter_trans_t;


/*  TRANSACTION TYPE = MO103 */

typedef struct alter_trans_mo103
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_3_s [3];
   give_up_member_t give_up_member;
   series_new_t series_new;
   quad_word order_number_u;
   uint32_t quantity_u;
   uint32_t condition_quantity_u;
   int32_t premium_i;
   char ex_client_s [10];
   uint16_t time_validity_n;
   uint8_t bid_or_ask_c;
   uint8_t order_type_c;
   uint8_t ignore_away_market_c;
   uint8_t client_category_c;
   uint8_t open_close_req_c;
   uint8_t bid_or_ask_next_c;
   uint8_t action_c;
   char filler_1_s;
   char broker_id_s [5];
   char free_text_s [15];
} alter_trans_mo103_t;


/*   TRANSACTION TYPE = MO259, MO299, MO387, MO427  */

typedef struct alter_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   quad_word order_number_u;
   order_var_t order_var;
} alter_trans_p_t;


/* TRANSACTION TYPE = CC15, CC271   */

typedef struct annul_exercise_req
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t exercise_number_i;
} annul_exercise_req_t;


/* TRANSACTION TYPE = CA18  */

typedef struct answer_account
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_account_item_t item [250];
} answer_account_t;


/*   TRANSACTION TYPE = CA38, CA275, CA294, CA2086  */

typedef struct answer_account_ext
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_account_ext_item_t item [230];
} answer_account_ext_t;


/*   TRANSACTION TYPE = DA13 */

typedef struct answer_account_fee_type
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_account_fee_type_item_t item [100];
} answer_account_fee_type_t;


/*   TRANSACTION TYPE = DA12 */

typedef struct answer_account_type
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_account_type_item_t item [100];
} answer_account_type_t;


/*   TRANSACTION TYPE = DA34  */

typedef struct answer_account_type_rule
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_account_type_rule_item_t item [100];
} answer_account_type_rule_t;


/*   TRANSACTION TYPE = DI3 */

typedef struct answer_add_tm_combo
{
   transaction_type_t transaction_type;
   da2_t da2;
} answer_add_tm_combo_t;


/* TRANSACTION TYPE = CA53  */

typedef struct answer_api_delivery
{
   transaction_type_t transaction_type;
   series_t series;
   char from_date_s [8];
   int32_t sequence_first_i;
   uint16_t items_n;
   char filler_2_s [2];
   answer_api_delivery_item_t item [450];
} answer_api_delivery_t;


/*   TRANSACTION TYPE = CA58  */

typedef struct answer_api_trade
{
   transaction_type_t transaction_type;
   series_t series;
   char from_date_s [8];
   int32_t sequence_first_i;
   uint16_t items_n;
   char filler_2_s [2];
   answer_api_trade_item_t item [180];
} answer_api_trade_t;


/*   TRANSACTION TYPE = CA11  */

typedef struct answer_api_trade_hdr
{
   transaction_type_t transaction_type;
   series_t series;
   char from_date_s [8];
   int32_t sequence_first_i;
   uint16_t items_n;
   char filler_2_s [2];
} answer_api_trade_hdr_t;


/*   TRANSACTION TYPE = UA10  */

typedef struct answer_bi26_signals_sent
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_bi26_signals_sent_item_t item [1000];
} answer_bi26_signals_sent_t;


/*   TRANSACTION TYPE = UA13  */

typedef struct answer_bi27_broadcasts_sent
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   char filler_1_s;
   uint8_t items_c;
   answer_bi27_broadcasts_sent_item1_t item1 [50];
} answer_bi27_broadcasts_sent_t;


/*   TRANSACTION TYPE = UA9  */

typedef struct answer_bi7_signals_sent
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_bi7_signals_sent_item_t item [1000];
} answer_bi7_signals_sent_t;


/*   TRANSACTION TYPE = UA11  */

typedef struct answer_bi8_broadcasts_sent
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   char filler_1_s;
   uint8_t items_c;
   answer_bi8_broadcasts_sent_item1_t item1 [50];
} answer_bi8_broadcasts_sent_t;


/*   TRANSACTION TYPE = DA211  */

typedef struct answer_bin_da211
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_bin_da211_item_t item [500];
} answer_bin_da211_t;


/*   TRANSACTION TYPE = MA99  */

typedef struct answer_block_size
{
   transaction_type_t transaction_type;
   int32_t max_block_order_size_i;
   int32_t max_block_price_size_i;
} answer_block_size_t;


/*   TRANSACTION TYPE = DA6  */

typedef struct answer_broker
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_1_s;
   uint16_t items_n;
   answer_broker_item_t item [50];
} answer_broker_t;


/*   TRANSACTION TYPE = DA206  */

typedef struct answer_broker_da206
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_1_s;
   uint16_t items_n;
   answer_broker_da206_item_t item [50];
} answer_broker_da206_t;


/*   TRANSACTION  TYPE = UA12  */

typedef struct answer_business_date
{
   transaction_type_t transaction_type;
   char omex_version_s [16];
   char business_date_s [8];
} answer_business_date_t;


/*   TRANSACTION TYPE = CA68  */

typedef struct answer_clearing_date
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   char omex_version_s [16];
   char business_date_s [8];
   uint16_t items_n;
   char filler_2_s [2];
   answer_clearing_date_item_t item [1000];
} answer_clearing_date_t;


/*   TRANSACTION TYPE = RA301  */

typedef struct answer_closing_series_prices
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_closing_series_prices_item_t item [500];
} answer_closing_series_prices_t;


/*  TRANSACTION TYPE = CA55, CA311, VA1, VA257  */

typedef struct answer_cl_underlying_delivery
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   char filler_2_s [2];
   answer_cl_underlying_delivery_item_t item [300];
} answer_cl_underlying_delivery_t;


/*   TRANSACTION TYPE = DA5  */

typedef struct answer_combo
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint8_t items_c;
   char filler_1_s;
   answer_combo_item_t item [100];
} answer_combo_t;


/*  TRANSACTION TYPE = MA133  */

typedef struct answer_combo_ob_ma133
{
   transaction_type_t transaction_type;
   series_next_t series_next;
   quad_word order_number_next_u;
   char orderbook_next_c;
   char filler_1_s;
   uint16_t items_n;
   answer_combo_ob_ma133_item_t item [177];
} answer_combo_ob_ma133_t;


/*  TRANSACTION TYPE = MA131  */

typedef struct answer_combo_order_ma131
{
   transaction_type_t transaction_type;
   series_next_t series_next;
   quad_word order_number_next_u;
   char orderbook_next_c;
   uint8_t bid_or_ask_next_c;
   uint16_t items_n;
   answer_combo_order_ma131_item_t item [165];
} answer_combo_order_ma131_t;


/*   TRANSACTION TYPE = CA64  */

typedef struct answer_commission
{
   transaction_type_t transaction_type;
   party_t party;
   uint8_t send_receive_c;
   char filler_1_s;
   uint16_t items_n;
   answer_commission_item_t item [800];
} answer_commission_t;


/*  TRANSACTION TYPE = CA62  */

typedef struct answer_conf_give_up_req_items
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   char filler_2_s [2];
   answer_conf_give_up_req_items_item_t item [50];
} answer_conf_give_up_req_items_t;


/*   TRANSACTION TYPE = DA15  */

typedef struct answer_converted_series
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_converted_series_item_t item [100];
} answer_converted_series_t;


/*  TRANSACTION TYPE = CA71  */

typedef struct answer_cover_req
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   char filler_2_s [2];
   answer_cover_req_item_t item [500];
} answer_cover_req_t;


/*  TRANSACTION TYPE = CA73  */

typedef struct answer_cover_req_upd
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   answer_cover_req_upd_item_t item [1000];
} answer_cover_req_upd_t;


/*   TRANSACTION TYPE = DA33  */

typedef struct answer_currency
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_currency_item_t item [100];
} answer_currency_t;


/*   TRANSACTION TYPE = IA41  */

typedef struct answer_daily_trade_statistics
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_daily_trade_statistics_item_t item [500];
} answer_daily_trade_statistics_t;


/*  TRANSACTION TYPE = CA23,CA24,CA31,CA280,CA287,CA1047,CA1048  */

typedef struct answer_delivery
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_delivery_item_t item [100];
} answer_delivery_t;


/*   TRANSACTION TYPE = DA212  */

typedef struct answer_dependencies_da212
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_dependencies_da212_item_t item [1000];
} answer_dependencies_da212_t;


/*   TRANSACTION TYPE = DA244 */

typedef struct answer_designated_exc_da244
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_designated_exc_da244_item_t item [1000];
} answer_designated_exc_da244_t;


/*   TRANSACTION TYPE = DA242 */

typedef struct answer_diss_group_da242
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_diss_group_da242_item_t item [1000];
} answer_diss_group_da242_t;


/*  TRANSACTION TYPE = UA16  */

typedef struct answer_dsg_exchg_list_status
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_dsg_exchg_list_status_item_t item [1000];
} answer_dsg_exchg_list_status_t;


/*  TRANSACTION TYPE = IA112  */

typedef struct answer_each_opra_prices_ia112
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_each_opra_prices_ia112_item_t item [1000];
} answer_each_opra_prices_ia112_t;


/* TRANSACTION TYPE = CA22, CA278, CA2070  */

typedef struct answer_error_msg
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_error_msg_item_t item [100];
} answer_error_msg_t;


/*   TRANSACTION TYPE = CA59  */

typedef struct answer_ever_last
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_ever_last_item_t item [3000];
} answer_ever_last_t;


/*   TRANSACTION TYPE = DA24  */

typedef struct answer_exchange_da24
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_exchange_da24_item_t item [100];
} answer_exchange_da24_t;


/*   TRANSACTION TYPE = RA31  */

typedef struct answer_exchange_rate
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_exchange_rate_item_t item [500];
} answer_exchange_rate_t;


/* TRANSACTION TYPE = CA21, CA277  */

typedef struct answer_exercise_req
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_exercise_req_item_t item [250];
} answer_exercise_req_t;


/*   TRANSACTION TYPE = IA2 */

typedef struct answer_extended_index
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   char filler_2_s [2];
   answer_extended_index_item_t item [700];
} answer_extended_index_t;


/*   TRANSACTION_TYPE = IA3 */

typedef struct answer_extended_underlying_info
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   char filler_2_s [2];
   answer_extended_underlying_info_item_t item [350];
} answer_extended_underlying_info_t;


/*   TRANSACTION TYPE = DA2063 */

typedef struct answer_external_id_da2063
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_external_id_da2063_item_t item [500];
} answer_external_id_da2063_t;


/*   TRANSACTION TYPE = DA17  */

typedef struct answer_ext_instrument
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint8_t items_c;
   char filler_1_s;
   answer_ext_instrument_item_t item [100];
} answer_ext_instrument_t;


/*   TRANSACTION TYPE = DA21  */

typedef struct answer_ext_inst_class
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_ext_inst_class_item_t item [100];
} answer_ext_inst_class_t;


/*   TRANSACTION TYPE = CA8  */

typedef struct answer_fixing_val
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_fixing_val_item_t item [500];
} answer_fixing_val_t;


/*  TRANSACTION TYPE = CA61  */

typedef struct answer_give_up_request
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_give_up_request_item_t item [480];
} answer_give_up_request_t;


/*   TRANSACTION TYPE = RA36  */

typedef struct answer_greeks
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   char created_time_s [6];
   char filler_2_s [2];
   answer_greeks_item_t item [1500];
} answer_greeks_t;


/*   TRANSACTION TYPE = HA1  */

typedef struct answer_hedge_prices
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_hedge_prices_item_t item [3000];
} answer_hedge_prices_t;


/*   TRANSACTION TYPE = CA28  */

typedef struct answer_historic_deal
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_historic_deal_item_t item [90];
} answer_historic_deal_t;


/*   TRANSACTION TYPE = MA2, MA12  */

typedef struct answer_inactive
{
   transaction_type_t transaction_type;
   uint8_t items_c;
   char filler_3_s [3];
   answer_inactive_item_t item [250];
} answer_inactive_t;


/*  TRANSACTION TYPE = MA132  */

typedef struct answer_inactive_combo_ma132
{
   transaction_type_t transaction_type;
   series_next_t series_next;
   quad_word order_number_next_u;
   char orderbook_next_c;
   uint8_t bid_or_ask_next_c;
   uint16_t items_n;
   answer_inactive_combo_ma132_item_t item [170];
} answer_inactive_combo_ma132_t;


/*   TRANSACTION TYPE = MA102  */

typedef struct answer_inactive_ma102
{
   transaction_type_t transaction_type;
   series_next_t series_next;
   quad_word order_number_next_u;
   uint16_t items_n;
   char orderbook_next_c;
   uint8_t bid_or_ask_next_c;
   answer_inactive_ma102_item_t item [700];
} answer_inactive_ma102_t;


/*   TRANSACTION TYPE = DA3, DA22 */

typedef struct answer_instrument
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_instrument_item_t item [100];
} answer_instrument_t;


/*   TRANSACTION TYPE = DA10, DA20, DA2055 */

typedef struct answer_instrument_class
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_instrument_class_item_t item [200];
} answer_instrument_class_t;


/*   TRANSACTION TYPE = DA210 */

typedef struct answer_instrument_class_da210
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_instrument_class_da210_item_t item [100];
} answer_instrument_class_da210_t;


/*   TRANSACTION TYPE = DA203 */

typedef struct answer_instrument_da203
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint8_t items_c;
   char filler_1_s;
   answer_instrument_da203_item_t item [100];
} answer_instrument_da203_t;


/*   TRANSACTION TYPE = DA8  */

typedef struct answer_instrument_group
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_instrument_group_item_t item [100];
} answer_instrument_group_t;


/*   TRANSACTION TYPE = DA208  */

typedef struct answer_instrument_group_da208
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_instrument_group_da208_item_t item [100];
} answer_instrument_group_da208_t;


/*   TRANSACTION TYPE = UA15  */

typedef struct answer_instrument_status
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_instrument_status_item_t item [1000];
} answer_instrument_status_t;


/*   TRANSACTION TYPE = DA240 */

typedef struct answer_inst_class_da240
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_inst_class_da240_item_t item [250];
} answer_inst_class_da240_t;


/*   TRANSACTION TYPE = IA46  */

typedef struct answer_interest_rate
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   char filler_2_s [2];
   answer_interest_rate_item_t item [300];
} answer_interest_rate_t;


/*   TRANSACTION TYPE = IA3  */

typedef struct answer_in_srv
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   char filler_2_s [2];
   answer_in_srv_item_t item [400];
} answer_in_srv_t;


/*   TRANSACTION TYPE = LA1  */

typedef struct answer_list
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t info_type_i;
   uint16_t segment_number_n;
   char list_name_s [40];
   uint16_t items_n;
   char text_buffer_s [50000];
} answer_list_t;


/*   TRANSACTION TYPE = MA106  */

typedef struct answer_locked_order_ma106
{
   transaction_type_t transaction_type;
   series_next_t series_next;
   quad_word order_number_next_u;
   uint8_t bid_or_ask_next_c;
   char orderbook_next_c;
   uint16_t items_n;
   answer_locked_order_ma106_item_t item [1000];
} answer_locked_order_ma106_t;


/*  TRANSACTION TYPE = SA2   */

typedef struct answer_manual_payments
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_manual_payments_item_t item [250];
} answer_manual_payments_t;


/*   TRANSACTION TYPE = RA5  */

typedef struct answer_margin
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_margin_item_t item [500];
} answer_margin_t;


/*   TRANSACTION TYPE = RA21  */

typedef struct answer_margin_acc
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_margin_acc_item_t item [500];
} answer_margin_acc_t;


/*   TRANSACTION TYPE = RA6  */

typedef struct answer_margin_ext
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_margin_ext_item_t item [500];
} answer_margin_ext_t;


/*   TRANSACTION TYPE = RA23, RA279  */

typedef struct answer_margin_mem
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_margin_mem_item_t item [500];
} answer_margin_mem_t;


/*   TRANSACTION TYPE = RA2  */

typedef struct answer_margin_param_block
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_margin_param_block_item_t item [100];
} answer_margin_param_block_t;


/*   TRANSACTION TYPE = RA20  */

typedef struct answer_margin_pa_acc
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_margin_pa_acc_item_t item [500];
} answer_margin_pa_acc_t;


/*   TRANSACTION TYPE = RA1  */

typedef struct answer_margin_series_param
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_margin_series_param_item_t item [500];
} answer_margin_series_param_t;


/*   TRANSACTION TYPE = RA42  */

typedef struct answer_margin_series_price
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_margin_series_price_item_t item [500];
} answer_margin_series_price_t;


/*  TRANSACTION TYPE = RA71  */

typedef struct answer_margin_simulation
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   char failure_reason_s [160];
   char filler_40_s [40];
   answer_margin_simulation_item_t item [500];
} answer_margin_simulation_t;


/*   TRANSACTION TYPE = RA41  */

typedef struct answer_margin_ulg_price
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_margin_ulg_price_item_t item [300];
} answer_margin_ulg_price_t;


/*   TRANSACTION TYPE = RA10  */

typedef struct answer_margin_vector
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_margin_vector_item_t item [1000];
} answer_margin_vector_t;


/*   TRANSACTION TYPE = RA11  */

typedef struct answer_margin_vector_alt
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   char created_time_s [6];
   char filler_2_s [2];
   answer_margin_vector_alt_item_t item [900];
} answer_margin_vector_alt_t;


/*   TRANSACTION TYPE = DA7, DA23  */

typedef struct answer_market
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_market_item_t item [100];
} answer_market_t;


/*   TRANSACTION TYPE = DA207 */

typedef struct answer_market_da207
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_market_da207_item_t item [100];
} answer_market_da207_t;


/*   TRANSACTION TYPE = DA2051 */

typedef struct answer_member_int
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_member_int_item_t item [100];
} answer_member_int_t;


/*   TRANSACTION TYPE = DA11  */

typedef struct answer_member_list
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_member_list_item_t item [100];
} answer_member_list_t;


/*   TRANSACTION TYPE = CA27  */

typedef struct answer_missing_deal
{
   transaction_type_t transaction_type;
   char filler_2_s [2];
   uint16_t items_n;
   answer_missing_deal_item_t item [90];
} answer_missing_deal_t;


/* TRANSACTION TYPE = CA52  */

typedef struct answer_missing_delivery
{
   transaction_type_t transaction_type;
   char filler_2_s [2];
   uint16_t items_n;
   answer_missing_delivery_item_t item [450];
} answer_missing_delivery_t;


/*   TRANSACTION TYPE = CA1  */

typedef struct answer_missing_trade
{
   transaction_type_t transaction_type;
   char filler_2_s [2];
   uint16_t items_n;
   answer_missing_trade_item_t item [180];
} answer_missing_trade_t;


/*   TRANSACTION TYPE = CA10  */

typedef struct answer_missing_trade_hdr
{
   transaction_type_t transaction_type;
   char filler_2_s [2];
   uint16_t items_n;
} answer_missing_trade_hdr_t;


/*  TRANSACTION TYPE = UA103 */

typedef struct answer_mm_exposure_limit_ua103
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_mm_exposure_limit_ua103_item_t item [1000];
} answer_mm_exposure_limit_ua103_t;


/*  TRANSACTION TYPE = UA101 */

typedef struct answer_mm_parameters_ua101
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_mm_parameters_ua101_item_t item [900];
} answer_mm_parameters_ua101_t;


/*   TRANSACTION TYPE = DA18  */

typedef struct answer_non_trading_days
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_non_trading_days_item_t item [100];
} answer_non_trading_days_t;


/*   TRANSACTION TYPE = MA16  */

typedef struct answer_ob
{
   transaction_type_t transaction_type;
   uint8_t items_c;
   char filler_3_s [3];
   answer_ob_item_t item [250];
} answer_ob_t;


/*   TRANSACTION TYPE = MA3, MA13, MA23  */

typedef struct answer_ob_id
{
   transaction_type_t transaction_type;
   uint8_t items_c;
   char filler_3_s [3];
   answer_ob_id_item_t item [250];
} answer_ob_id_t;


/*   TRANSACTION TYPE = MA103  */

typedef struct answer_ob_ma103
{
   transaction_type_t transaction_type;
   series_next_t series_next;
   quad_word order_number_next_u;
   uint8_t bid_or_ask_next_c;
   char filler_1_s;
   uint16_t items_n;
   answer_ob_ma103_item_t item [1000];
} answer_ob_ma103_t;


/*   TRANSACTION TYPE = IA1  */

typedef struct answer_ob_volumes
{
   transaction_type_t transaction_type;
   uint8_t items_c;
   char filler_3_s [3];
   answer_ob_volumes_item_t item [250];
} answer_ob_volumes_t;


/* TRANSACTION TYPE = CA20   */

typedef struct answer_open_interest
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint8_t items_c;
   char filler_1_s;
   answer_open_interest_item_t item [500];
} answer_open_interest_t;


/* TRANSACTION TYPE = CA72, CA328, CA2120   */

typedef struct answer_open_interest_ext
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint8_t items_c;
   char filler_1_s;
   answer_open_interest_ext_item_t item [278];
} answer_open_interest_ext_t;


/*  TRANSACTION TYPE = IA111 */

typedef struct answer_opra_prices_ia111
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_opra_prices_ia111_item_t item [1000];
} answer_opra_prices_ia111_t;


/*   TRANSACTION TYPE = MA1, MA11, MA21  */

typedef struct answer_order
{
   transaction_type_t transaction_type;
   uint8_t items_c;
   char filler_3_s [3];
   answer_order_item_t item [250];
} answer_order_t;


/*   TRANSACTION TYPE = MA101, MA357  */

typedef struct answer_order_ma101
{
   transaction_type_t transaction_type;
   series_next_t series_next;
   quad_word order_number_next_u;
   uint16_t items_n;
   char orderbook_next_c;
   uint8_t bid_or_ask_next_c;
   answer_order_ma101_item_t item [700];
} answer_order_ma101_t;


/*  TRANSACTION TYPE = UA1  */

typedef struct answer_partition
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_partition_item_t item [100];
} answer_partition_t;


/*   TRANSACTION TYPE = MA6, MA9  */

typedef struct answer_party
{
   transaction_type_t transaction_type;
   uint8_t items_c;
   char filler_3_s [3];
   answer_party_item_t item [250];
} answer_party_t;


/* TRANSACTION TYPE = SA1  */

typedef struct answer_paynote_info
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_paynote_info_item_t item [250];
} answer_paynote_info_t;


/*  TRANSACTION TYPE = CA51, VA2, VA258  */

typedef struct answer_physical_delivery
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_physical_delivery_item_t item [200];
} answer_physical_delivery_t;


/*  TRANSACTION TYPE = UA102 */

typedef struct answer_pmm_parameters_ua102
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_pmm_parameters_ua102_item_t item [400];
} answer_pmm_parameters_ua102_t;


/*   TRANSACTION TYPE = CA3,CA65,CA259,CA300,CA321,CA1027,CA2058,CA2113  */

typedef struct answer_position
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_position_item_t item [500];
} answer_position_t;


/*   TRANSACTION TYPE = IA17  */

typedef struct answer_prel_settlement
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   uint16_t segment_number_n;
   answer_prel_settlement_item_t item [2000];
} answer_prel_settlement_t;


/*  TRANSACTION TYPE = IA101  */

typedef struct answer_prices_ia101
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_prices_ia101_item_t item [840];
} answer_prices_ia101_t;


/*  TRANSACTION TYPE = IA102  */

typedef struct answer_price_depth_ia102
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_price_depth_ia102_item_t item [3000];
} answer_price_depth_ia102_t;


/*   TRANSACTION TYPE = IA2  */

typedef struct answer_price_info
{
   transaction_type_t transaction_type;
   uint8_t items_c;
   char filler_3_s [3];
   answer_price_info_item_t item [250];
} answer_price_info_t;


/* TRANSACTION TYPE = CA19   */

typedef struct answer_propagate
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_propagate_item_t item [100];
} answer_propagate_t;


/*   TRANSACTION TYPE = MA8, MA18, MA28 */

typedef struct answer_proxy
{
   transaction_type_t transaction_type;
   series_t series;
   quad_word order_number_u;
   uint16_t items_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
   answer_proxy_item_t item [450];
} answer_proxy_t;


/*   TRANSACTION TYPE = MA104  */

typedef struct answer_quote_ma104
{
   transaction_type_t transaction_type;
   char orderbook_next_c;
   char filler_3_s [3];
   series_next_t series_next;
   quad_word order_number_next_u;
   uint8_t bid_or_ask_next_c;
   char filler_1_s;
   uint16_t items_n;
   answer_quote_ma104_item_t item [1000];
} answer_quote_ma104_t;


/*  TRANSACTION TYPE = RA44, RA300  */

typedef struct answer_realtime_ulg_price
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_realtime_ulg_price_item_t item [300];
} answer_realtime_ulg_price_t;


/* TRANSACTION TYPE = CA16, CA272   */

typedef struct answer_rectify_d
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   char reserved_2_s [2];
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t items_n;
   char filler_2_s [2];
   answer_rectify_d_item_t item [100];
} answer_rectify_d_t;


/* TRANSACTION TYPE = CA17, CA131, CA273   */

typedef struct answer_rectify_d_cont
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   char filler_2_s [2];
   answer_rectify_d_cont_item_t item [255];
} answer_rectify_d_cont_t;


/* TRANSACTION TYPE = CA15, CA271  */

typedef struct answer_rectify_ext_cont
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   char filler_2_s [2];
   answer_rectify_ext_cont_item_t item [100];
} answer_rectify_ext_cont_t;


/* TRANSACTION TYPE = CA14   */

typedef struct answer_rectify_t
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   char reserved_2_s [2];
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t items_n;
   char filler_2_s [2];
   answer_rectify_t_item_t item [100];
} answer_rectify_t_t;


/*   TRANSACTION TYPE = LA2, LA258   */

typedef struct answer_report
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_report_item_t item [351];
} answer_report_t;


/*  TRANSACTION TYPE = MA114 */

typedef struct answer_rotation_ma114
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   char filler_2_s [2];
   answer_rotation_ma114_item_t item [100];
} answer_rotation_ma114_t;


/*   TRANSACTION TYPE = DA2  */

typedef struct answer_series
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_series_item_t item [400];
} answer_series_t;


/*   TRANSACTION TYPE = DA9  */

typedef struct answer_series_bo
{
   transaction_type_t transaction_type;
   char date_trading_s [8];
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_series_bo_item_t item [400];
} answer_series_bo_t;


/*   TRANSACTION TYPE = DA202  */

typedef struct answer_series_da202
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_series_da202_item_t item [1000];
} answer_series_da202_t;


/*   TRANSACTION TYPE = DA232  */

typedef struct answer_series_da232
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_series_da232_item_t item [800];
} answer_series_da232_t;


/*   TRANSACTION TYPE = DA16 */

typedef struct answer_series_delivery
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_series_delivery_item_t item [10];
} answer_series_delivery_t;


/*   TRANSACTION TYPE = MA4  */

typedef struct answer_status
{
   transaction_type_t transaction_type;
   uint8_t country_c;
   uint8_t market_c;
   uint8_t on_off_c;
   char filler_1_s;
} answer_status_t;


/* TRANSACTION TYPE = CA49, CA305   */

typedef struct answer_tm_deal
{
   transaction_type_t transaction_type;
   series_t series;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_tm_deal_item_t item [380];
} answer_tm_deal_t;


/*   TRANSACTION TYPE = IB12  */

typedef struct answer_tot_equil_prices
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint8_t instance_c;
   uint8_t instance_next_c;
   series_next_t series_next;
   uint16_t items_n;
   char filler_2_s [2];
   answer_tot_equil_prices_item_t item [3000];
} answer_tot_equil_prices_t;


/*   TRANSACTION TYPE = MA44, MA54, MA64  */

typedef struct answer_tot_inactive
{
   transaction_type_t transaction_type;
   series_t series;
   quad_word order_number_u;
   uint16_t items_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
   answer_tot_inactive_item_t item [450];
} answer_tot_inactive_t;


/*   TRANSACTION TYPE = MA42, MA52, MA62  */

typedef struct answer_tot_ob
{
   transaction_type_t transaction_type;
   series_t series;
   quad_word order_number_u;
   uint16_t items_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
   answer_tot_ob_item_t item [1000];
} answer_tot_ob_t;


/*   TRANSACTION TYPE = MA41, MA51, MA61  */

typedef struct answer_tot_ob_id
{
   transaction_type_t transaction_type;
   series_t series;
   quad_word order_number_u;
   uint16_t items_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
   answer_tot_ob_id_item_t item [500];
} answer_tot_ob_id_t;


/*   TRANSACTION TYPE = MA43, MA53, MA55, MA63, MA311  */

typedef struct answer_tot_order
{
   transaction_type_t transaction_type;
   series_t series;
   quad_word order_number_u;
   uint16_t items_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
   answer_tot_order_item_t item [450];
} answer_tot_order_t;


/*   TRANSACTION TYPE = MA10, MA45  */

typedef struct answer_tot_party
{
   transaction_type_t transaction_type;
   series_t series;
   quad_word order_number_u;
   uint16_t items_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
   answer_tot_party_item_t item [500];
} answer_tot_party_t;


/*   TRANSACTION TYPE = IA10  */

typedef struct answer_tot_volume_prices
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint8_t instance_c;
   uint8_t instance_next_c;
   series_next_t series_next;
   uint16_t items_n;
   char filler_2_s [2];
   answer_tot_volume_prices_item_t item [380];
} answer_tot_volume_prices_t;


/*   TRANSACTION TYPE = IA11  */

typedef struct answer_tot_volume_prices_bbo
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint8_t instance_c;
   uint8_t instance_next_c;
   series_next_t series_next;
   uint16_t items_n;
   char filler_2_s [2];
   answer_tot_volume_prices_bbo_item_t item [900];
} answer_tot_volume_prices_bbo_t;


/*   TRANSACTION TYPE = CA2, CA13, CA269, CA299, CA1026  */

typedef struct answer_trade
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_trade_item_t item [180];
} answer_trade_t;


/*   TRANSACTION TYPE = IA45  */

typedef struct answer_trade_deriv_history
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_trade_deriv_history_item_t item [1000];
} answer_trade_deriv_history_t;


/* TRANSACTION TYPE = CA257, CA258, CA292  */

typedef struct answer_trade_int
{
   transaction_type_t transaction_type;
   partition_low_t partition_low;
   partition_high_t partition_high;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_trade_int_item_t item [180];
} answer_trade_int_t;


/*   TRANSACTION TYPE = IA42  */

typedef struct answer_trade_statistics
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_trade_statistics_item_t item [500];
} answer_trade_statistics_t;


/*   TRANSACTION TYPE = IA44  */

typedef struct answer_trade_uv_history
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_trade_uv_history_item_t item [1000];
} answer_trade_uv_history_t;


/*   TRANSACTION TYPE = DA29  */

typedef struct answer_trading_state
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_trading_state_item_t item [100];
} answer_trading_state_t;


/*   TRANSACTION TYPE = DA245  */

typedef struct answer_trading_state_da245
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_trading_state_da245_item_t item [100];
} answer_trading_state_da245_t;


/*   TRANSACTION TYPE = DA4, DA19 */

typedef struct answer_underlying
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_underlying_item_t item [50];
} answer_underlying_t;


/*   TRANSACTION TYPE = DA14  */

typedef struct answer_underlying_adjustment
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_underlying_adjustment_item_t item [100];
} answer_underlying_adjustment_t;


/*   TRANSACTION TYPE = DA204  */

typedef struct answer_underlying_da204
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint8_t items_c;
   char filler_1_s;
   answer_underlying_da204_item_t item [100];
} answer_underlying_da204_t;


/*   TRANSACTION TYPE = IA12  */

typedef struct answer_underlying_indices
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   uint16_t segment_number_n;
   answer_underlying_indices_item_t item [1000];
} answer_underlying_indices_t;


/*  TRANSACTION TYPE = IA121 */

typedef struct answer_underlying_prices_ia121
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_underlying_prices_ia121_item_t item [1000];
} answer_underlying_prices_ia121_t;


/*  TRANSACTION TYPE = IA132 */

typedef struct answer_underlying_status_us
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_underlying_status_us_item_t item [5000];
} answer_underlying_status_us_t;


/*   TRANSACTION TYPE = DA30 */

typedef struct answer_user_type_info
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   char ust_id_s [5];
   uint8_t ext_or_int_c;
   uint8_t is_trader_c;
   char filler_1_s;
   uint32_t min_ord_percent_u;
   uint32_t max_ord_percent_u;
   answer_user_type_info_item_t item [100];
} answer_user_type_info_t;


/*   TRANSACTION TYPE = IA4  */

typedef struct answer_uv_price
{
   transaction_type_t transaction_type;
   uint8_t items_c;
   char filler_3_s [3];
   answer_uv_price_item_t item [100];
} answer_uv_price_t;


/*   TRANSACTION TYPE = IA4  */

typedef struct answer_volume_prices
{
   transaction_type_t transaction_type;
   uint8_t items_c;
   char filler_3_s [3];
   answer_volume_prices_item_t item [250];
} answer_volume_prices_t;


/*   TRANSACTION TYPE = DA243 */

typedef struct answer_vol_diss_step_da243
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   answer_vol_diss_step_da243_item_t item [300];
} answer_vol_diss_step_da243_t;


/*   TRANSACTION TYPE = MA107  */

typedef struct ans_pmm_price_imp_quote_ma107
{
   transaction_type_t transaction_type;
   char orderbook_next_c;
   char filler_3_s [3];
   series_next_t series_next;
   quad_word order_number_next_u;
   uint8_t bid_or_ask_next_c;
   char filler_1_s;
   uint16_t items_n;
   ans_pmm_price_imp_quote_ma107_item_t item [700];
} ans_pmm_price_imp_quote_ma107_t;


/*   TRANSACTION TYPE = UI1  */

typedef struct application_status
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t application_status_i;
} application_status_t;


/*  BROADCAST TYPE = BI131, BI133  */

typedef struct away_adm_msg_update_bi131
{
   broadcast_type_t broadcast_type;
   int64_t timestamp_q;
   char msg_s [1000];
   uint8_t country_c;
   uint8_t feeder_type_c;
   char filler_2_s [2];
   char reserved_8_s [8];
} away_adm_msg_update_bi131_t;


/*  BROADCAST TYPE = BI114 */

typedef struct away_market_flag_update_us
{
   broadcast_type_t broadcast_type;
   uint16_t items_n;
   char filler_2_s [2];
   away_market_flag_update_us_item_t item [300];
} away_market_flag_update_us_t;


/*   BROADCAST TYPE = BO11 BO21   */

typedef struct bb_ob_changes_id
{
   broadcast_type_t broadcast_type;
   changes_t changes;
   quad_word order_number_u;
   order_t order;
   uint8_t items_c;
   char filler_3_s [3];
   bb_ob_changes_id_item_t item [9];
} bb_ob_changes_id_t;


/*   BROADCAST TYPE = BO12   */

typedef struct bb_ob_changes_no_id
{
   broadcast_type_t broadcast_type;
   changes_t changes;
   quad_word order_number_u;
   bb_order_no_id_t bb_order_no_id;
   uint8_t items_c;
   char filler_3_s [3];
   bb_ob_changes_no_id_item_t item [9];
} bb_ob_changes_no_id_t;


/*  BROADCAST TYPE = BO101 */

typedef struct best_bid_offer_bo101
{
   broadcast_type_t broadcast_type;
   int32_t reserved_i;
   uint16_t items_n;
   char filler_2_s [2];
   best_bid_offer_bo101_item_t item [43];
} best_bid_offer_bo101_t;


/*  BROADCAST TYPE = BO119 */

typedef struct best_bid_offer_bo119
{
   broadcast_type_t broadcast_type;
   int32_t reserved_i;
   uint16_t items_n;
   char filler_2_s [2];
   best_bid_offer_bo119_item_t item [69];
} best_bid_offer_bo119_t;


/*  BROADCAST TYPE = BO102 */

typedef struct best_bid_offer_extended_bo102
{
   broadcast_type_t broadcast_type;
   int32_t reserved_i;
   uint16_t items_n;
   char filler_2_s [2];
   best_bid_offer_extended_bo102_item_t item [30];
} best_bid_offer_extended_bo102_t;


/*  TRANSACTION TYPE = MO107 */

typedef struct block_order_mo107
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_3_s [3];
   int32_t premium_i;
   uint32_t quantity_u;
   uint16_t time_validity_n;
   uint8_t bid_or_ask_c;
   uint8_t client_category_c;
   uint8_t open_close_req_c;
   uint8_t expose_flag_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
} block_order_mo107_t;


/*   BROADCAST TYPE = BO99  */

typedef struct block_order_response
{
   broadcast_type_t broadcast_type;
   quad_word order_number_u;
   uint8_t items_c;
   char filler_3_s [3];
   block_order_response_item_t item [100];
} block_order_response_t;


/*   TRANSACTION TYPE = MO32  */

typedef struct block_order_trans
{
   transaction_type_t transaction_type;
   series_t series;
   give_up_member_t give_up_member;
   char exchange_info_s [16];
   uint8_t items_c;
   char filler_3_s [3];
   block_order_trans_item_t item [100];
} block_order_trans_t;


/*   TRANSACTION TYPE = MO288, MO416  */

typedef struct block_order_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   give_up_member_t give_up_member;
   char exchange_info_s [16];
   uint8_t items_c;
   char filler_3_s [3];
   block_order_trans_p_item_t item [100];
} block_order_trans_p_t;


/*   TRANSACTION TYPE = MO36  */

typedef struct block_price_trans
{
   transaction_type_t transaction_type;
   series_t series;
   give_up_member_t give_up_member;
   char exchange_info_s [16];
   char customer_info_s [15];
   uint8_t items_c;
   block_price_trans_item_t item [100];
} block_price_trans_t;


/*   TRANSACTION TYPE = MO292, MO420,   */

typedef struct block_price_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   give_up_member_t give_up_member;
   char exchange_info_s [16];
   char customer_info_s [15];
   uint8_t items_c;
   block_price_trans_p_item_t item [100];
} block_price_trans_p_t;


/*   BROADCAST TYPE = BI28, BI284 */

typedef struct bond_index_params
{
   broadcast_type_t broadcast_type;
   uint16_t commodity_n;
   char filler_2_s [2];
   int32_t duration_i;
   int32_t internal_interest_rate_i;
} bond_index_params_t;


/*  BROADCAST TYPE = BD6, BO14   */

typedef struct broadcast_hdr
{
   broadcast_type_t broadcast_type;
   uint16_t items_n;
   uint16_t size_n;
} broadcast_hdr_t;


/*   TRANSACTION TYPE = MO5  */

typedef struct broker_trans
{
   transaction_type_t transaction_type;
   series_t series;
   order_var_t order_var;
   party_t party;
} broker_trans_t;


/*   TRANSACTION TYPE = MO261, MO389  */

typedef struct broker_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   order_var_t order_var;
   party_t party;
} broker_trans_p_t;


/*  TRANSACTION TYPE = MO110 */

typedef struct cab_order_mo110
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   uint8_t bid_or_ask_c;
   uint8_t client_category_c;
   uint8_t open_close_req_c;
   uint32_t quantity_u;
   uint16_t time_validity_n;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
} cab_order_mo110_t;


/*  TRANSACTION TYPE = CC48, CC304  */

typedef struct cancel_cover_req
{
   transaction_type_t transaction_type;
   series_t series;
   account_t account;
   uint32_t cover_request_nbr_u;
} cancel_cover_req_t;


/*   TRANSACTION TYPE = CC19 */

typedef struct cancel_trade
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t trade_number_i;
   int32_t ext_seq_nbr_i;
} cancel_trade_t;


/*   TRANSACTION TYPE = CA11???  */

typedef struct ced_answer_daily_trade_stat
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   uint16_t items_n;
   ced_answer_daily_trade_stat_item_t item [500];
} ced_answer_daily_trade_stat_t;


/*  BROADCAST TYPE = BI27 */

typedef struct clearing_message
{
   broadcast_type_t broadcast_type;
   uint16_t broadcast_number_n;
   uint8_t country_c;
   uint8_t market_c;
   uint16_t items_n;
   clearing_message_item_t item [10];
} clearing_message_t;


/* TRANSACTION TYPE = CD128, CD6   */

typedef struct cl_rectify_trade
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t reserved_i;
   int32_t change_operation_i;
   int32_t trade_number_i;
   uint8_t items_c;
   char filler_3_s [3];
   cl_rectify_trade_item_t item [100];
} cl_rectify_trade_t;


/*   TRANSACTION TYPE = CD4  */

typedef struct cl_reregistration
{
   transaction_type_t transaction_type;
   series_t series;
   char reserved_12_s [12];
   uint8_t items_c;
   char filler_3_s [3];
   cl_reregistration_item_t item [100];
} cl_reregistration_t;


/*   TRANSACTION TYPE = CD5, CD261  */

typedef struct cl_reregistration_bo
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t items_c;
   char filler_3_s [3];
   cl_reregistration_bo_item_t item [100];
} cl_reregistration_bo_t;


/*   subrecord  */

typedef struct cl_trade_base_api
{
   trading_code_t trading_code;
   series_t series;
   give_up_member_t give_up_member;
   quad_word order_number_u;
   int32_t sequence_number_i;
   int32_t trade_number_i;
   int32_t deal_price_i;
   int32_t trade_quantity_i;
   account_t account;
   char customer_info_s [15];
   uint8_t bought_or_sold_c;
   uint8_t deal_source_c;
   uint8_t open_close_req_c;
   uint8_t trade_type_c;
   char filler_1_s;
   user_code_t user_code;
   char created_date_s [8];
   char created_time_s [6];
   char asof_date_s [8];
   char asof_time_s [6];
   char modified_date_s [8];
   char modified_time_s [6];
   uint8_t trade_state_c;
   uint8_t attention_c;
   int32_t deal_number_i;
   uint32_t global_deal_no_u;
   int32_t orig_trade_number_i;
   orig_series_t orig_series;
   char exchange_info_s [16];
} cl_trade_base_api_t;


/*   TRANSACTION TYPE = MO19  */

typedef struct combo_acc_trans
{
   transaction_type_t transaction_type;
   series_t series;
   order_var_t order_var;
   quad_word order_number_u;
} combo_acc_trans_t;


/*   TRANSACTION TYPE = MO275, MO403 */

typedef struct combo_acc_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   order_var_t order_var;
   quad_word order_number_u;
} combo_acc_trans_p_t;


/*  BROADCAST TYPE = BO131  */

typedef struct combo_ob_changes_bo131
{
   broadcast_type_t broadcast_type;
   trading_code_t trading_code;
   int32_t premium_i;
   int32_t ul_price_low_i;
   int32_t ul_price_high_i;
   quad_word order_number_u;
   uint32_t quantity_u;
   uint16_t time_validity_n;
   uint16_t commodity_n;
   char message_s [100];
   char reserved_32_s [32];
   uint8_t change_reason_c;
   uint8_t order_type_c;
   uint8_t interest_order_c;
   uint8_t items_c;
   char ul_price_method_c;
   char filler_3_s [3];
   combo_ob_changes_bo131_item_t item [10];
} combo_ob_changes_bo131_t;


/*  BROADCAST TYPE = BO132  */

typedef struct combo_ob_member_bo132
{
   broadcast_type_t broadcast_type;
   trading_code_t trading_code;
   int32_t premium_i;
   char ex_client_s [10];
   uint16_t time_validity_n;
   int32_t ul_price_low_i;
   int32_t ul_price_high_i;
   quad_word order_number_u;
   uint32_t quantity_u;
   uint16_t commodity_n;
   uint16_t reserved_4_n;
   uint8_t change_reason_c;
   uint8_t order_type_c;
   uint8_t interest_order_c;
   uint8_t client_category_c;
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char message_s [100];
   char reserved_32_s [32];
   uint8_t items_c;
   char ul_price_method_c;
   char filler_2_s [2];
   combo_ob_member_bo132_item_t item [10];
} combo_ob_member_bo132_t;


/*  TRANSACTION TYPE = MO131  */

typedef struct combo_order_trans_mo131
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_3_s [3];
   char ex_client_s [10];
   uint16_t time_validity_n;
   int32_t premium_i;
   int32_t ul_price_low_i;
   int32_t ul_price_high_i;
   uint32_t quantity_u;
   uint16_t reserved_4_n;
   uint8_t order_type_c;
   uint8_t interest_order_c;
   uint8_t client_category_c;
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char message_s [100];
   char reserved_32_s [32];
   char ul_price_method_c;
   char filler_1_s;
   uint8_t items_c;
   combo_order_trans_mo131_item_t item [10];
} combo_order_trans_mo131_t;


/*   TRANSACTION TYPE = MO2, MO12, MO20, MO22  */

typedef struct combo_trans
{
   transaction_type_t transaction_type;
   series_t series;
   order_var_t order_var;
} combo_trans_t;


/*   TRANSACTION TYPE = MO258,MO278,MO386,MO396,MO404,MO406  */

typedef struct combo_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   order_var_t order_var;
} combo_trans_p_t;


/*   BROADCAST TYPE = BU5 */

typedef struct combo_update_bu5
{
   broadcast_type_t broadcast_type;
   uint16_t chg_type_n;
   char filler_2_s [2];
   da5_t da5;
} combo_update_bu5_t;


/*  TRANSACTION TYPE = CC2096  */

typedef struct confirm_cover_req
{
   transaction_type_t transaction_type;
   series_t series;
   account_t account;
   uint32_t cover_request_nbr_u;
   uint8_t confirm_or_reject_c;
   char filler_3_s [3];
} confirm_cover_req_t;


/*  TRANSACTION TYPE = CC38, CC39  */

typedef struct confirm_give_up_request
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t give_up_number_i;
   uint16_t items_n;
   char filler_2_s [2];
   confirm_give_up_request_item_t item [50];
} confirm_give_up_request_t;


/* TRANSACTION TYPE = CC12, CC268, CD132  */

typedef struct confirm_rectify_d
{
   transaction_type_t transaction_type;
   series_t series;
   int64_t rectify_deal_number_q;
   uint8_t operation_c;
   char confirm_reject_c;
   char filler_2_s [2];
} confirm_rectify_d_t;


/* TRANSACTION TYPE = CC11, CC267   */

typedef struct confirm_rectify_t
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t rectify_trade_number_i;
   char confirm_reject_c;
   char filler_3_s [3];
} confirm_rectify_t_t;


/* TRANSACTION TYPE = CD8, CD130  */

typedef struct countersign_trade
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t trade_number_i;
   int32_t ext_status_i;
   char confirm_reject_c;
   char filler_3_s [3];
} countersign_trade_t;


/*  BROADCAST TYPE = BD24  */

typedef struct cover_rec_bc
{
   broadcast_type_t broadcast_type;
   cover_data_item_t cover_data_item;
} cover_rec_bc_t;


/*  BROADCAST TYPE = BD26  */

typedef struct cover_rec_upd_bc
{
   broadcast_type_t broadcast_type;
   cover_state_item_t cover_state_item;
} cover_rec_upd_bc_t;


/*  TRANSACTION TYPE = CC47, CC303  */

typedef struct cover_req
{
   transaction_type_t transaction_type;
   series_t series;
   account_t account;
   int32_t cover_quantity_i;
} cover_req_t;


/*  TRANSACTION TYPE = MO108 */

typedef struct crossing_order_mo108
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   uint8_t allow_price_impr_c;
   uint8_t expose_flag_c;
   uint8_t special_order_type_c;
   int32_t premium_i;
   originating_side_t originating_side;
   counter_party_side_t counter_party_side;
} crossing_order_mo108_t;


/* TRANSACTION TYPE = UO2 */

typedef struct crossing_report
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t premium_i;
   uint16_t mp_quantity_n;
   uint16_t block_n;
   uint16_t buy_time_validity_n;
   uint16_t sell_time_validity_n;
   char buy_ex_client_s [10];
   char sell_ex_client_s [10];
   char customer_info_s [15];
   uint8_t open_close_req_c;
} crossing_report_t;


/*  BROADCAST TYPE = BI37  */

typedef struct crossing_report_info
{
   broadcast_type_t broadcast_type;
   series_t series;
   uint16_t block_n;
   uint16_t mp_quantity_n;
   uint16_t seconds_n;
   char filler_2_s [2];
} crossing_report_info_t;


/* TRANSACTION TYPE = UO1 */

typedef struct crossing_report_trans
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t premium_i;
   uint16_t mp_quantity_n;
   uint16_t block_n;
   char buy_ex_client_s [10];
   char sell_ex_client_s [10];
   char buy_customer_info_s [15];
   char sell_customer_info_s [15];
   uint8_t buy_open_close_req_c;
   uint8_t sell_open_close_req_c;
} crossing_report_trans_t;


/*   TRANSACTION TYPE = MC2  */

typedef struct crossing_request
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t block_n;
   uint16_t mp_quantity_n;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} crossing_request_t;


/*   BROADCAST TYPE = BD17  */

typedef struct dc_deal_user
{
   broadcast_type_t broadcast_type;
   series_t series;
   int32_t deal_price_i;
   uint16_t deal_quantity_n;
   uint8_t hidden_price_c;
   uint8_t items_c;
   dc_deal_user_item_t item [100];
} dc_deal_user_t;


/*   BROADCAST TYPE = BD1, BD82  */

typedef struct deal_user
{
   broadcast_type_t broadcast_type;
   series_t series;
   uint32_t sequence_number_u;
   int32_t deal_price_i;
   uint16_t deal_quantity_n;
   uint16_t segment_number_n;
   char filler_2_s [2];
   uint8_t hidden_price_c;
   uint8_t items_c;
   deal_user_item_t item [84];
} deal_user_t;


/*   BROADCAST TYPE = BD111  */

typedef struct deal_user_bd111
{
   broadcast_type_t broadcast_type;
   series_t series;
   int32_t deal_price_i;
   uint32_t deal_quantity_u;
   uint8_t deal_source_c;
   uint8_t items_c;
   char filler_2_s [2];
   deal_user_bd111_item_t item [80];
} deal_user_bd111_t;


/*  BROADCAST TYPE = BL22 */

typedef struct dedicated_mm_alarm_bl22
{
   broadcast_type_t broadcast_type;
   series_t series;
   uint32_t mmsup_status_u;
   uint32_t alarm_status_u;
   trading_code_t trading_code;
   uint16_t block_n;
   char filler_2_s [2];
} dedicated_mm_alarm_bl22_t;


/*   BROADCAST TYPE = BL8  */

typedef struct dedic_quote_request_vol_info
{
   broadcast_type_t broadcast_type;
   series_t series;
   uint16_t block_n;
   uint16_t mp_quantity_n;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} dedic_quote_request_vol_info_t;


/*   TRANSACTION TYPE = MO34, MO35  */

typedef struct delete_all_trans
{
   transaction_type_t transaction_type;
   series_t series;
} delete_all_trans_t;


/*   TRANSACTION TYPE = MO290, MO291, MO418, MO419  */

typedef struct delete_all_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
} delete_all_trans_p_t;


/*   TRANSACTION TYPE = MO4, MO14 ,MO24 , MO40, MO44, MO54, MO56  */

typedef struct delete_trans
{
   transaction_type_t transaction_type;
   series_t series;
   quad_word order_number_u;
   whose_t whose;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} delete_trans_t;


/*  TRANSACTION TYPE = MO104,MO105,MO360  */

typedef struct delete_trans_mo104_mo105
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_3_s [3];
   quad_word order_number_u;
   whose_t whose;
   uint16_t order_category_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
} delete_trans_mo104_mo105_t;


/*   TRANSACTION TYPE = MO260,MO280,MO300,MO310,MO312,MO388,MO398,MO408,MO424,MO428,MO438,MO440  */

typedef struct delete_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   quad_word order_number_u;
   whose_t whose;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} delete_trans_p_t;


/*  BROADCAST TYPE = BO116 */

typedef struct derived_order_bo116
{
   broadcast_type_t broadcast_type;
   series_t series;
   quad_word order_number_u;
   int32_t premium_i;
   uint32_t quantity_u;
   uint8_t bid_or_ask_c;
   uint8_t action_derived_order_c;
   char filler_2_s [2];
} derived_order_bo116_t;


/*  BROADCAST TYPE = BI104 */

typedef struct de_series_status_bi104
{
   broadcast_type_t broadcast_type;
   uint16_t items_n;
   char filler_2_s [2];
   de_series_status_bi104_item_t item [69];
} de_series_status_bi104_t;


/*   BROADCAST TYPE =BD9, BD10, BD11  */

typedef struct directed_deal
{
   broadcast_type_t broadcast_type;
   directed_deal_item_t item [2];
} directed_deal_t;


/* BROADCAST TYPE = BD18  */

typedef struct directed_delivery
{
   broadcast_type_t broadcast_type;
   cl_delivery_api_t cl_delivery_api;
} directed_delivery_t;


/*   BROADCAST TYPE = BD4, BD5  */

typedef struct directed_trade
{
   broadcast_type_t broadcast_type;
   cl_trade_api_t cl_trade_api;
} directed_trade_t;


/* TRANSACTION TYPE = CC296, VC260  */

typedef struct distribute_physical_delivery
{
   transaction_type_t transaction_type;
   series_t series;
} distribute_physical_delivery_t;


/*  BROADCAST TYPE = BI42  */

typedef struct dsg_exchg_list_status_info
{
   broadcast_type_t broadcast_type;
   uint16_t items_n;
   char filler_2_s [2];
   dsg_exchg_list_status_info_item_t item [85];
} dsg_exchg_list_status_info_t;


/*   TRANSACTION TYPE = MC3  */

typedef struct end_crossing
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t block_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
} end_crossing_t;


/*   BROADCAST TYPE = BO10  */

typedef struct equil_price_update
{
   broadcast_type_t broadcast_type;
   uint16_t items_n;
   char filler_2_s [2];
   equil_price_update_item_t item [50];
} equil_price_update_t;


/*  TRANSACTION TYPE = MO136  */

typedef struct execute_combo_trans_mo136
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   uint8_t client_category_c;
   uint8_t step_up_to_bbo_c;
   uint8_t step_up_odd_tick_c;
   int32_t premium_i;
   char ex_client_s [10];
   uint16_t time_validity_n;
   quad_word order_number_u;
   uint32_t quantity_u;
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   uint8_t items_c;
   char filler_3_s [3];
   execute_combo_trans_mo136_item_t item [10];
} execute_combo_trans_mo136_t;


/*  TRANSACTION TYPE =  MO122  */

typedef struct execution_report_list_mo122
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_1_s;
   uint16_t items_n;
   execution_report_list_mo122_item_t item [200];
} execution_report_list_mo122_t;


/* TRANSACTION TYPE = CC13, CC269, CC2050   */

typedef struct exercise_req
{
   transaction_type_t transaction_type;
   series_t series;
   account_t account;
   int32_t quantity_i;
   int32_t trade_number_i;
} exercise_req_t;


/*   BROADCAST TYPE = BI17, BI273 */

typedef struct extended_index
{
   broadcast_type_t broadcast_type;
   char index_s [15];
   char last_index_s [8];
   char high_index_s [8];
   char low_index_s [8];
   char change_previous_s [8];
   char change_yesterday_s [8];
   char timestamp_dist_s [5];
   char timestamp_comp_s [5];
   char significant_s [8];
   char trend_indicator_c;
   char filler_2_s [2];
} extended_index_t;


/*   BROADCAST TYPE = BI18, BI274 */

typedef struct extended_underlying_info
{
   broadcast_type_t broadcast_type;
   int32_t opening_ref_price_i;
   int32_t theoretical_opening_price_i;
   int32_t theoretical_closing_price_i;
   uint32_t traded_volume_opening_u;
   uint32_t theoretical_volume_opening_u;
   uint32_t traded_volume_closing_u;
   int32_t low_price_i;
   int32_t high_price_i;
   int32_t ref_price_i;
   int32_t last_price_i;
   uint32_t volume_u;
   uint32_t turnover_u;
   uint16_t commodity_n;
   char filler_2_s [2];
   uint32_t ape_bid_mask_u;
   uint32_t ape_ask_mask_u;
   char isin_code_s [12];
   char last_deal_time_s [6];
   uint8_t on_off_c;
   char trading_phase_code_c;
   char trend_indicator_c;
   char filler_1_s;
   uint16_t items_n;
   extended_underlying_info_item_t item [5];
} extended_underlying_info_t;


/*   TRANSACTION TYPE = CA12 */

typedef struct ext_answer_missing_trade_proxy
{
   transaction_type_t transaction_type;
   char yymmdd_s [6];
   uint16_t items_n;
   ext_answer_missing_trade_proxy_item_t item [350];
} ext_answer_missing_trade_proxy_t;


/*  BROADCAST TYPE = BO125  */

typedef struct ext_quote_reject_reason_bo125
{
   broadcast_type_t broadcast_type;
   quad_word order_number_u;
   char filler_2_s [2];
   uint16_t items_n;
   ext_quote_reject_reason_bo125_item_t item [150];
} ext_quote_reject_reason_bo125_t;


/*   BROADCAST TYPE = BI8 */

typedef struct general_message
{
   broadcast_type_t broadcast_type;
   uint8_t country_c;
   uint8_t market_c;
   uint16_t items_n;
   general_message_item_t item [10];
} general_message_t;


/*  TRANSACTION TYPE = CD35, CD291  */

typedef struct give_up_request
{
   transaction_type_t transaction_type;
   series_t series;
   account_t account;
   int32_t trade_number_i;
   int32_t trade_quantity_i;
   int32_t commission_i;
   char give_up_text_s [30];
   char filler_2_s [2];
} give_up_request_t;


/*   BROADCAST TYPE = BD28  */

typedef struct hedge_price_update
{
   broadcast_type_t broadcast_type;
   uint16_t items_n;
   char filler_2_s [2];
   hedge_price_update_item_t item [50];
} hedge_price_update_t;


/*   TRANSACTION TYPE = MO31  */

typedef struct hv_order_trans
{
   transaction_type_t transaction_type;
   series_t series;
   order_var_t order_var;
   give_up_member_t give_up_member;
   char exchange_info_s [16];
   uint16_t total_volume_n;
   char filler_2_s [2];
} hv_order_trans_t;


/*   TRANSACTION TYPE = MO287, MO415  */

typedef struct hv_order_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   order_var_t order_var;
   give_up_member_t give_up_member;
   char exchange_info_s [16];
   uint16_t total_volume_n;
   char filler_2_s [2];
} hv_order_trans_p_t;


/*   TRANSACTION TYPE = MO37  */

typedef struct hv_price_2_trans
{
   transaction_type_t transaction_type;
   series_t series;
   give_up_member_t give_up_member;
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   uint16_t bid_quantity_n;
   uint16_t ask_quantity_n;
   uint16_t bid_total_volume_n;
   uint16_t ask_total_volume_n;
   uint16_t block_n;
   uint16_t time_validity_n;
   quad_word order_number_bid_u;
   quad_word order_number_ask_u;
   char ex_client_s [10];
   uint8_t order_type_c;
   char customer_info_s [15];
   char filler_2_s [2];
   char exchange_info_s [16];
} hv_price_2_trans_t;


/*   TRANSACTION TYPE = MO293, MO421, MO450  */

typedef struct hv_price_2_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   give_up_member_t give_up_member;
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   uint16_t bid_quantity_n;
   uint16_t ask_quantity_n;
   uint16_t bid_total_volume_n;
   uint16_t ask_total_volume_n;
   uint16_t block_n;
   uint16_t time_validity_n;
   quad_word order_number_bid_u;
   quad_word order_number_ask_u;
   char ex_client_s [10];
   uint8_t order_type_c;
   char customer_info_s [15];
   char filler_2_s [2];
   char exchange_info_s [16];
} hv_price_2_trans_p_t;


/*   BROADCAST TYPE = BI5, BI261 */

typedef struct index_info
{
   broadcast_type_t broadcast_type;
   char index_s [15];
   char last_index_s [8];
   char high_index_s [8];
   char low_index_s [8];
   char change_previous_s [8];
   char change_yesterday_s [8];
   char timestamp_dist_s [5];
   char timestamp_comp_s [5];
   char filler_3_s [3];
} index_info_t;


/*   BROADCAST TYPE = BI9  */

typedef struct info_heartbeat
{
   broadcast_type_t broadcast_type;
   uint8_t heartbeat_interval_c;
   uint8_t instance_c;
   uint8_t tot_instances_c;
   char description_s [40];
   char filler_1_s;
} info_heartbeat_t;


/*   BROADCAST TYPE = BI13  */

typedef struct info_heartbeat_ext
{
   broadcast_type_t broadcast_type;
   uint8_t heartbeat_interval_c;
   uint8_t orderbook_high_c;
   uint8_t orderbook_low_c;
   uint8_t tot_instances_c;
   char description_s [40];
} info_heartbeat_ext_t;


/*   BROADCAST TYPE = BI7  */

typedef struct info_ready
{
   broadcast_type_t broadcast_type;
   int32_t info_type_i;
   series_t series;
   char business_date_s [8];
   char sent_date_s [8];
   char sent_time_s [6];
   char filler_2_s [2];
} info_ready_t;


/*   BROADCAST TYPE = BI210 */

typedef struct instrument_class_update_bi210
{
   broadcast_type_t broadcast_type;
   uint16_t chg_type_n;
   char filler_2_s [2];
   da210_t da210;
} instrument_class_update_bi210_t;


/*   BROADCAST TYPE = BI41  */

typedef struct instrument_status_info
{
   broadcast_type_t broadcast_type;
   uint16_t items_n;
   char filler_2_s [2];
   instrument_status_info_item_t item [12];
} instrument_status_info_t;


/*   BROADCAST TYPE = BU10, BU20 */

typedef struct inst_class_update_bu10_bu20
{
   broadcast_type_t broadcast_type;
   uint16_t chg_type_n;
   char filler_2_s [2];
   da10_da20_t da10_da20;
} inst_class_update_bu10_bu20_t;


/* TRANSACTION TYPE = BD6   */

typedef struct item_hdr
{
   uint16_t items_n;
   uint16_t size_n;
} item_hdr_t;


/*  BROADCAST TYPE = BO115 */

typedef struct locked_order_bo115
{
   broadcast_type_t broadcast_type;
   series_t series;
   trading_code_t trading_code;
   quad_word order_number_u;
   int32_t premium_i;
   int32_t premium_best_own_exchange_i;
   int32_t premium_far_away_exchange_i;
   uint32_t quantity_u;
   uint16_t quantity_at_best_premium_n;
   uint16_t time_validity_n;
   uint16_t order_category_n;
   uint8_t country_c;
   uint8_t bid_or_ask_c;
   uint8_t order_type_c;
   uint8_t ignore_away_market_c;
   uint8_t client_category_c;
   uint8_t lock_type_c;
   uint8_t action_locked_order_c;
   char filler_3_s [3];
} locked_order_bo115_t;


/*  TRANSACTION TYPE = CD38, CD294, CD2086  */

typedef struct long_position_adj
{
   transaction_type_t transaction_type;
   series_t series;
   char filler_2_s [2];
   uint16_t items_n;
   long_position_adj_item_t item [1500];
} long_position_adj_t;


/*   BROADCAST TYPE = MI3  */

typedef struct market_established
{
   broadcast_type_t broadcast_type;
   series_t series;
   uint16_t block_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
   int32_t bid_premium_i;
   int32_t ask_premium_i;
} market_established_t;


/*   BROADCAST TYPE = BI10  */

typedef struct market_hours_mp
{
   broadcast_type_t broadcast_type;
   trading_code_t trading_code;
   sts_gen_t sts_gen;
} market_hours_mp_t;


/*  BROADCAST TYPE = BI101 */

typedef struct message_bi101
{
   broadcast_type_t broadcast_type;
   uint16_t broadcast_number_n;
   series_t series;
   uint8_t country_c;
   char filler_3_s [3];
   uint16_t items_n;
   message_bi101_item_t item [10];
} message_bi101_t;


/*   BROADCAST TYPE = MI2  */

typedef struct mmsup_alarm
{
   broadcast_type_t broadcast_type;
   series_t series;
   trading_code_t trading_code;
   uint32_t mmsup_status_u;
   uint16_t block_n;
   char filler_2_s [2];
} mmsup_alarm_t;


/*   BROADCAST TYPE = BI22  */

typedef struct mmsup_ext_alarm
{
   broadcast_type_t broadcast_type;
   series_t series;
   trading_code_t trading_code;
   uint32_t mmsup_status_u;
   uint32_t alarm_status_u;
   uint16_t block_n;
   char filler_2_s [2];
} mmsup_ext_alarm_t;


/*   TRANSACTION TYPE = CC22, CC2070  */

typedef struct modify_account
{
   transaction_type_t transaction_type;
   series_t series;
   account_t account;
   account_data_t account_data;
} modify_account_t;


/*   TRANSACTION TYPE = CC41  */

typedef struct modify_commission
{
   transaction_type_t transaction_type;
   series_t series;
   party_t party;
   uint8_t send_receive_c;
   char filler_1_s;
   uint16_t items_n;
   modify_commission_item_t item [800];
} modify_commission_t;


/*   TRANSACTION TYPE = CD1, CD259, CD513 */

typedef struct mp_deal
{
   transaction_type_t transaction_type;
   series_t series;
   int64_t timestamp_log_q;
   uint32_t sequence_number_u;
   uint8_t sequence_id_c;
   uint8_t items_c;
   uint8_t poss_dup_c;
   uint8_t account_validation_c;
   mp_deal_item_t item [98];
} mp_deal_t;


/*  TRANSACTION TYPE = MO124  */

typedef struct multiple_order_trans_mo124
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_1_s;
   uint16_t items_n;
   multiple_order_trans_mo124_item_t item [200];
} multiple_order_trans_mo124_t;


/*  BROADCAST TYPE = BO130  */

typedef struct ob_changes_bo130
{
   broadcast_type_t broadcast_type;
   trading_code_t trading_code;
   series_t series;
   give_up_member_t give_up_member;
   int32_t premium_i;
   quad_word order_number_u;
   uint32_t quantity_u;
   uint16_t time_validity_n;
   uint16_t order_category_n;
   uint8_t order_type_c;
   uint8_t client_category_c;
   uint8_t ignore_away_market_c;
   uint8_t change_reason_c;
   uint8_t bid_or_ask_c;
   uint8_t open_close_req_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
} ob_changes_bo130_t;


/*   BROADCAST TYPE = BO1  */

typedef struct ob_changes_id
{
   broadcast_type_t broadcast_type;
   changes_t changes;
   int64_t timestamp_log_q;
   quad_word order_number_u;
   order_t order;
} ob_changes_id_t;


/*   BROADCAST TYPE = BO2, BO7  */

typedef struct ob_changes_no_id
{
   broadcast_type_t broadcast_type;
   changes_t changes;
   quad_word order_number_u;
   order_no_id_t order_no_id;
} ob_changes_no_id_t;


/*   BROADCAST TYPE = BO14 */

typedef struct ob_levels_id
{
   series_t series;
   uint16_t block_n;
   char filler_2_s [2];
} ob_levels_id_t;


/*   BROADCAST TYPE = BO14 */

typedef struct ob_levels_order_number
{
   quad_word order_number_bid_u;
   quad_word order_number_ask_u;
} ob_levels_order_number_t;


/*   BROADCAST TYPE = BO14 */

typedef struct ob_levels_price_volumes
{
   uint16_t bid_mask_n;
   uint16_t ask_mask_n;
   uint8_t price_level_c;
   uint8_t items_c;
   char filler_2_s [2];
   ob_levels_price_volumes_item_t item [32];
} ob_levels_price_volumes_t;


/*   BROADCAST TYPE = BO14 */

typedef struct ob_levels_remaining_quantity
{
   uint32_t rem_quantity_bid_u;
   uint32_t rem_quantity_ask_u;
} ob_levels_remaining_quantity_t;


/*   BROADCAST TYPE = BO14 */

typedef struct ob_levels_sequence_number
{
   uint32_t sequence_number_u;
} ob_levels_sequence_number_t;


/*   BROADCAST TYPE = BO33  */

typedef struct ob_volumes_bbo
{
   broadcast_type_t broadcast_type;
   uint16_t items_n;
   char filler_2_s [2];
   ob_volumes_bbo_item_t item [41];
} ob_volumes_bbo_t;


/*  BROADCAST TYPE = BO104 */

typedef struct opra_all_best_bid_offer_bo104
{
   broadcast_type_t broadcast_type;
   char filler_2_s [2];
   uint16_t items_n;
   opra_all_best_bid_offer_bo104_item_t item [30];
} opra_all_best_bid_offer_bo104_t;


/*  BROADCAST TYPE = BO103 */

typedef struct opra_best_bid_offer_bo103
{
   broadcast_type_t broadcast_type;
   char filler_2_s [2];
   uint16_t items_n;
   opra_best_bid_offer_bo103_item_t item [30];
} opra_best_bid_offer_bo103_t;


/*  BROADCAST TYPE = BD102 */

typedef struct opra_trade_info_bd102
{
   broadcast_type_t broadcast_type;
   series_t series;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   uint32_t volume_u;
   uint32_t turnover_u;
   int32_t reserved_1_i;
   int32_t reserved_2_i;
   uint8_t country_c;
   uint8_t away_market_status_c;
   uint8_t out_of_sequence_c;
   char filler_1_s;
} opra_trade_info_bd102_t;


/*  BROADCAST TYPE = BO118 */

typedef struct orders_to_be_hedged_bo118
{
   broadcast_type_t broadcast_type;
   series_t series;
   trading_code_t trading_code;
   int32_t de_premium_i;
   int32_t orginal_premium_i;
   uint32_t orginal_quantity_u;
   quad_word order_number_u;
   uint32_t de_quantity_u;
   uint32_t time_received_u;
   uint8_t bid_or_ask_c;
   uint8_t orderbook_c;
   uint8_t country_c;
   uint8_t responsible_to_hedge_c;
   char filler_2_s [2];
   uint16_t items_n;
   orders_to_be_hedged_bo118_item_t item [100];
} orders_to_be_hedged_bo118_t;


/*   TRANSACTION TYPE = MO18  */

typedef struct order_acc_trans
{
   transaction_type_t transaction_type;
   series_t series;
   order_var_t order_var;
   quad_word order_number_u;
} order_acc_trans_t;


/*   TRANSACTION TYPE = MO274, MO402  */

typedef struct order_acc_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   order_var_t order_var;
   quad_word order_number_u;
} order_acc_trans_p_t;


/*  BROADCAST TYPE = BO107 */

typedef struct order_quote_deleted_bo107
{
   broadcast_type_t broadcast_type;
   series_t series;
   quad_word order_number_u;
   int32_t premium_i;
   uint16_t rem_quantity_n;
   uint16_t order_category_n;
   uint8_t bid_or_ask_c;
   uint8_t reason_order_delete_c;
   char filler_2_s [2];
} order_quote_deleted_bo107_t;


/*   TRANSACTION TYPE = MO1,MO7, MO8, MO9, MO11, MO21  */

typedef struct order_trans
{
   transaction_type_t transaction_type;
   series_t series;
   order_var_t order_var;
} order_trans_t;


/*  TRANSACTION TYPE = MO101 */

typedef struct order_trans_mo101
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_3_s [3];
   uint32_t quantity_u;
   int32_t premium_i;
   uint16_t time_validity_n;
   uint8_t bid_or_ask_c;
   uint8_t order_type_c;
   uint8_t ignore_away_market_c;
   uint8_t client_category_c;
   uint8_t open_close_req_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   char filler_2_s [2];
   char filler_1_s;
} order_trans_mo101_t;


/*   TRANSACTION TYPE = MO257,MO277,MO385,MO395,MO405  */

typedef struct order_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   order_var_t order_var;
} order_trans_p_t;


/*   BROADCAST TYPE = BI26  */

typedef struct pay_note_info_ready
{
   broadcast_type_t broadcast_type;
   int32_t info_type_i;
   char settlement_date_s [8];
   char clh_id_s [12];
   char sent_date_s [8];
   char sent_time_s [6];
   char filler_2_s [2];
} pay_note_info_ready_t;


/*  TRANSACTION TYPE = MO123  */

typedef struct pmm_price_imp_quote_mo123
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_1_s;
   uint16_t items_n;
   pmm_price_imp_quote_mo123_item_t item [200];
} pmm_price_imp_quote_mo123_t;


/*  TRANSACTION TYPE = MO111 */

typedef struct pmm_release_locked_order_mo111
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_3_s [3];
   quad_word order_number_u;
   uint8_t bid_or_ask_c;
   uint8_t lock_type_c;
   char filler_2_s [2];
} pmm_release_locked_order_mo111_t;


/*  TRANSACTION TYPE = MO112 */

typedef struct pmm_trade_report_trans_mo112
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_3_s [3];
   quad_word order_number_u;
   uint32_t quantity_u;
   int32_t premium_i;
   uint16_t order_category_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
} pmm_trade_report_trans_mo112_t;


/*   TRANSACTION TYPE = MO66  */

typedef struct price_2_trans
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   uint16_t bid_quantity_n;
   uint16_t ask_quantity_n;
   uint16_t block_n;
   uint16_t time_validity_n;
   quad_word order_number_bid_u;
   quad_word order_number_ask_u;
   char ex_client_s [10];
   char filler_2_s [2];
} price_2_trans_t;


/*   TRANSACTION TYPE = MO322, MO450  */

typedef struct price_2_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   uint16_t bid_quantity_n;
   uint16_t ask_quantity_n;
   uint16_t block_n;
   uint16_t time_validity_n;
   quad_word order_number_bid_u;
   quad_word order_number_ask_u;
   char ex_client_s [10];
   char filler_2_s [2];
} price_2_trans_p_t;


/*  BROADCAST TYPE = BO117  */

typedef struct price_depth_info_bo117
{
   broadcast_type_t broadcast_type;
   int32_t reserved_i;
   uint16_t items_n;
   char filler_2_s [2];
   price_depth_info_bo117_item_t item [80];
} price_depth_info_bo117_t;


/*   BROADCAST TYPE = BD2  */

typedef struct price_info
{
   broadcast_type_t broadcast_type;
   series_t series;
   char trend_indicator_c;
   uint8_t deal_source_c;
   char all_or_none_c;
   char filler_1_s;
   int32_t reserved_i;
   int32_t closing_price_i;
   int32_t opening_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   uint32_t volume_u;
   uint32_t turnover_u;
   uint32_t open_balance_u;
   uint32_t number_of_deals_u;
} price_info_t;


/*   TRANSACTION TYPE = MO6  */

typedef struct price_trans
{
   transaction_type_t transaction_type;
   series_t series;
   order_var_t order_var;
   quad_word order_number_u;
} price_trans_t;


/*   TRANSACTION TYPE = MO262, MO390  */

typedef struct price_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   order_var_t order_var;
   quad_word order_number_u;
} price_trans_p_t;


/* TRANSACTION TYPE = CQ18, CQ37, CQ38, CQ294, CQ2095, CQ2086     */

typedef struct query_account
{
   transaction_type_t transaction_type;
   series_t series;
   account_t account;
   uint16_t segment_number_n;
   uint8_t query_on_date_c;
   char date_s [8];
   char filler_1_s;
} query_account_t;


/*   TRANSACTION TYPE = DQ13 */

typedef struct query_account_fee_type
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_account_fee_type_t;


/* TRANSACTION TYPE = CQ19, CQ275  */

typedef struct query_account_prop
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   account_t account;
   char filler_2_s [2];
} query_account_prop_t;


/*   TRANSACTION TYPE = DQ12 */

typedef struct query_account_type
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_account_type_t;


/*   TRANSACTION TYPE = DQ34 */

typedef struct query_account_type_rule
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_account_type_rule_t;


/* TRANSACTION TYPE = CQ53  */

typedef struct query_api_delivery
{
   transaction_type_t transaction_type;
   series_t series;
   char from_date_s [8];
   int32_t sequence_first_i;
   char to_date_s [8];
   int32_t sequence_last_i;
} query_api_delivery_t;


/*   TRANSACTION TYPE = CQ11, CQ58  */

typedef struct query_api_trade
{
   transaction_type_t transaction_type;
   series_t series;
   char from_date_s [8];
   int32_t sequence_first_i;
   char to_date_s [8];
   int32_t sequence_last_i;
} query_api_trade_t;


/*   TRANSACTION TYPE = UQ10  */

typedef struct query_bi26_signals_sent
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   char settlement_date_s [8];
   char filler_2_s [2];
} query_bi26_signals_sent_t;


/*   TRANSACTION TYPE = UQ13  */

typedef struct query_bi27_broadcasts_sent
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_bi27_broadcasts_sent_t;


/*   TRANSACTION TYPE = UQ9  */

typedef struct query_bi7_signals_sent
{
   transaction_type_t transaction_type;
   search_series_t search_series;
   uint16_t segment_number_n;
   char business_date_s [8];
   char filler_2_s [2];
} query_bi7_signals_sent_t;


/*   TRANSACTION TYPE = UQ11  */

typedef struct query_bi8_broadcasts_sent
{
   transaction_type_t transaction_type;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_bi8_broadcasts_sent_t;


/*   TRANSACTION TYPE = DQ211  */

typedef struct query_bin_dq211
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_3_s [3];
} query_bin_dq211_t;


/*   TRANSACTION TYPE = MQ99  */

typedef struct query_block_size
{
   transaction_type_t transaction_type;
   series_t series;
} query_block_size_t;


/*  TRANSACTION TYPE = DQ6, DQ206 */

typedef struct query_broker
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_3_s [3];
} query_broker_t;


/*   TRANSACTION  TYPE = UQ12  */

typedef struct query_business_date
{
   transaction_type_t transaction_type;
} query_business_date_t;


/*   TRANSACTION TYPE = CQ68  */

typedef struct query_clearing_date
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
} query_clearing_date_t;


/*   TRANSACTION TYPE = RQ301  */

typedef struct query_closing_series_price
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   uint16_t only_expiring_today_n;
   char ins_id_s [32];
   char date_s [8];
} query_closing_series_price_t;


/* TRANSACTION TYPE = CQ55, CQ311, VQ1, VQ257  */

typedef struct query_cl_underlying_delivery
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t state_c;
   char filler_3_s [3];
} query_cl_underlying_delivery_t;


/*   TRANSACTION TYPE = DQ5  */

typedef struct query_combo
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_combo_t;


/*  TRANSACTION TYPE = MQ133  */

typedef struct query_combo_ob_mq133
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   uint8_t bin_c;
   uint8_t bid_or_ask_c;
   char filler_1_s;
   uint16_t order_category_n;
   char filler_2_s [2];
   whose_t whose;
   quad_word order_number_u;
} query_combo_ob_mq133_t;


/*  TRANSACTION TYPE = MQ131  */

typedef struct query_combo_order_mq131
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   uint8_t bin_c;
   uint8_t bid_or_ask_c;
   char filler_1_s;
   uint16_t order_category_n;
   char filler_2_s [2];
   whose_t whose;
   quad_word order_number_u;
} query_combo_order_mq131_t;


/*   TRANSACTION TYPE = CQ64  */

typedef struct query_commission
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t send_receive_c;
   party_t party;
   char filler_3_s [3];
} query_commission_t;


/*  TRANSACTION TYPE = CQ62  */

typedef struct query_conf_give_up_req_items
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t give_up_number_i;
} query_conf_give_up_req_items_t;


/*   TRANSACTION TYPE = DQ15  */

typedef struct query_converted_series
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   uint16_t adjust_ident_n;
} query_converted_series_t;


/*  TRANSACTION TYPE = CQ71, CQ327  */

typedef struct query_cover_req
{
   transaction_type_t transaction_type;
   series_t series;
   uint32_t seq_nbr_1_u;
   uint32_t seq_nbr_2_u;
   char business_date_s [8];
} query_cover_req_t;


/*  TRANSACTION TYPE = CQ73  */

typedef struct query_cover_req_upd
{
   transaction_type_t transaction_type;
   series_t series;
   uint32_t seq_nbr_1_u;
   uint32_t seq_nbr_2_u;
   char business_date_s [8];
} query_cover_req_upd_t;


/*   TRANSACTION TYPE = DQ33 */

typedef struct query_currency
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_currency_t;


/* TRANSACTION TYPE = CQ23, CQ24  */

typedef struct query_delivery
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16_t segment_number_n;
   char filler_2_s [2];
   int32_t event_type_i;
   int32_t class_no_i;
   char date_s [8];
} query_delivery_t;


/*   TRANSACTION TYPE = DQ212  */

typedef struct query_dependencies_dq212
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_dependencies_dq212_t;


/*  TRANSACTION TYPE = DQ244  */

typedef struct query_designated_exc_dq244
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_designated_exc_dq244_t;


/*  TRANSACTION TYPE = DQ242  */

typedef struct query_diss_group_dq242
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_diss_group_dq242_t;


/*  TRANSACTION TYPE = UQ16  */

typedef struct query_dsg_exchg_list_status
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_dsg_exchg_list_status_t;


/*  TRANSACTION TYPE = IQ112  */

typedef struct query_each_opra_prices_iq112
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16_t segment_number_n;
   uint8_t bin_c;
   char filler_1_s;
} query_each_opra_prices_iq112_t;


/* TRANSACTION TYPE = CQ22, CQ278, CQ2070  */

typedef struct query_error_msg
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16_t segment_number_n;
   account_t account;
   char from_date_s [8];
   char to_date_s [8];
   char from_time_s [6];
   char to_time_s [6];
   char filler_2_s [2];
   uint32_t error_id_u;
} query_error_msg_t;


/*   TRANSACTION TYPE = CQ59  */

typedef struct query_ever_last
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   char filler_2_s [2];
} query_ever_last_t;


/*   TRANSACTION TYPE = DQ24  */

typedef struct query_exchange_dq24
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_exchange_dq24_t;


/*   TRANSACTION TYPE = RQ31  */

typedef struct query_exchange_rate
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   char filler_2_s [2];
} query_exchange_rate_t;


/* TRANSACTION TYPE = CQ21, CQ277   */

typedef struct query_exercise_req
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16_t segment_number_n;
   account_t account;
   char filler_2_s [2];
} query_exercise_req_t;


/*   TRANSACTION TYPE = II2, II258 */

typedef struct query_extended_index
{
   transaction_type_t transaction_type;
   series_t series;
   char date_s [8];
} query_extended_index_t;


/*   TRANSACTION TYPE = II3, II259 */

typedef struct query_extended_underlying_info
{
   transaction_type_t transaction_type;
   series_t series;
   char date_s [8];
} query_extended_underlying_info_t;


/*  TRANSACTION TYPE = DQ2063  */

typedef struct query_external_id_dq2063
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_external_id_dq2063_t;


/*   TRANSACTION TYPE = CQ8  */

typedef struct query_fixing_val
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16_t segment_number_n;
   char date_s [8];
   char filler_2_s [2];
} query_fixing_val_t;


/*  TRANSACTION TYPE = CQ61  */

typedef struct query_give_up_request
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   uint8_t state_c;
   char created_date_s [8];
   char series_id_s [32];
   char filler_1_s;
} query_give_up_request_t;


/*   TRANSACTION TYPE = RQ36  */

typedef struct query_greeks
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   uint8_t intra_day2_c;
   char filler_1_s;
} query_greeks_t;


/*   TRANSACTION TYPE = HQ1  */

typedef struct query_hedge_prices
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_hedge_prices_t;


/*   TRANSACTION TYPE = CQ28  */

typedef struct query_historic_deal
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16_t segment_number_n;
   char from_date_s [8];
   char from_time_s [6];
   char to_date_s [8];
   char to_time_s [6];
   char filler_2_s [2];
} query_historic_deal_t;


/* TRANSACTION TYPE = CQ13, CQ269   */

typedef struct query_holding_trade
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16_t segment_number_n;
   account_t account;
   char filler_2_s [2];
} query_holding_trade_t;


/*   TRANSACTION TYPE = MQ2, MQ12  */

typedef struct query_inactive
{
   transaction_type_t transaction_type;
   series_t series;
   whose_t whose;
} query_inactive_t;


/*  TRANSACTION TYPE = MQ132  */

typedef struct query_inactive_combo_mq132
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   uint8_t bin_c;
   uint8_t bid_or_ask_c;
   char filler_1_s;
   uint16_t order_category_n;
   char filler_2_s [2];
   whose_t whose;
   quad_word order_number_u;
} query_inactive_combo_mq132_t;


/*   TRANSACTION TYPE = MQ102  */

typedef struct query_inactive_mq102
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_3_s [3];
   whose_t whose;
   quad_word order_number_u;
   uint16_t order_category_n;
   uint8_t bin_c;
   uint8_t bid_or_ask_c;
   filter_series_t filter_series;
} query_inactive_mq102_t;


/*   TRANSACTION TYPE = DQ3, DQ17, DQ22, DQ25, DQ203, DQ153, DQ172 */

typedef struct query_instrument
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_instrument_t;


/*  TRANSACTION TYPE = DQ10, DQ20, DQ21, DQ110, DQ210, DQ160, DQ170 */

typedef struct query_instrument_class
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_instrument_class_t;


/*  TRANSACTION TYPE = DQ8, DQ208 */

typedef struct query_instrument_group
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_instrument_group_t;


/*   TRANSACTION TYPE = UQ15  */

typedef struct query_instrument_status
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_instrument_status_t;


/*  TRANSACTION TYPE = DQ240  */

typedef struct query_inst_class_dq240
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_inst_class_dq240_t;


/*   TRANSACTION TYPE = IQ46  */

typedef struct query_interest_rate
{
   transaction_type_t transaction_type;
   series_t series;
} query_interest_rate_t;


/*   TRANSACTION TYPE = LQ1  */

typedef struct query_list
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char yymmdd_s [6];
   int32_t info_type_i;
} query_list_t;


/*   TRANSACTION TYPE = MQ106  */

typedef struct query_locked_order_mq106
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   whose_t whose;
   quad_word order_number_u;
   uint8_t bin_c;
   uint8_t bid_or_ask_c;
   char filler_1_s;
} query_locked_order_mq106_t;


/* TRANSACTION TYPE = SQ2  */

typedef struct query_manual_payments
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
   char date_s [8];
} query_manual_payments_t;


/*   TRANSACTION TYPE = RQ21  */

typedef struct query_margin_acc
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   account_t account;
   char cust_bank_id_s [12];
   char filler_2_s [2];
} query_margin_acc_t;


/*   TRANSACTION TYPE = RQ6  */

typedef struct query_margin_ext
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   char filler_2_s [2];
} query_margin_ext_t;


/*   TRANSACTION TYPE = RQ23  */

typedef struct query_margin_mem
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   char filler_2_s [2];
} query_margin_mem_t;


/*   TRANSACTION TYPE = RQ2  */

typedef struct query_margin_param_block
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_margin_param_block_t;


/*   TRANSACTION TYPE = RQ20  */

typedef struct query_margin_pa_acc
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   account_t account;
   char cust_bank_id_s [12];
   char filler_2_s [2];
} query_margin_pa_acc_t;


/*   TRANSACTION TYPE = RQ1030  */

typedef struct query_margin_proxy
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   char country_id_s [2];
   char ex_customer_s [5];
   char filler_3_s [3];
} query_margin_proxy_t;


/*   TRANSACTION TYPE = RQ1  */

typedef struct query_margin_series_param
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_margin_series_param_t;


/*   TRANSACTION TYPE = RQ42  */

typedef struct query_margin_series_price
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   char filler_2_s [2];
} query_margin_series_price_t;


/*   TRANSACTION TYPE = RQ71  */

typedef struct query_margin_simulation
{
   transaction_type_t transaction_type;
   series_t series;
   account_t account;
   uint16_t segment_number_n;
   uint16_t qry_segment_number_n;
   uint16_t items_n;
   uint8_t pos_sim_c;
   uint8_t price_sim_c;
   uint8_t vol_sim_c;
   uint8_t output_level_c;
   uint8_t last_qry_segment_c;
   char filler_1_s;
   char date_s [8];
   char filler_40_s [40];
   query_margin_simulation_item_t item [1000];
} query_margin_simulation_t;


/*   TRANSACTION TYPE = RQ41  */

typedef struct query_margin_ulg_price
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   char filler_2_s [2];
} query_margin_ulg_price_t;


/*   TRANSACTION TYPE = RQ10  */

typedef struct query_margin_vector
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   char filler_2_s [2];
} query_margin_vector_t;


/*   TRANSACTION TYPE = RQ11  */

typedef struct query_margin_vector_alt
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   uint8_t intra_day_c;
   char filler_1_s;
} query_margin_vector_alt_t;


/*  TRANSACTION TYPE = DQ7, DQ23, DQ207 */

typedef struct query_market
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_market_t;


/*   TRANSACTION TYPE = DQ2051 */

typedef struct query_member_int
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_member_int_t;


/*   TRANSACTION TYPE = DQ11 */

typedef struct query_member_list
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_member_list_t;


/*   TRANSACTION TYPE = CQ27 */

typedef struct query_missing_deal
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t sequence_first_i;
   int32_t sequence_last_i;
   char date_s [8];
} query_missing_deal_t;


/* TRANSACTION TYPE = CQ52  */

typedef struct query_missing_delivery
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t sequence_first_i;
   int32_t sequence_last_i;
   char date_s [8];
} query_missing_delivery_t;


/*   TRANSACTION TYPE = CQ1, CQ10  */

typedef struct query_missing_trade
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t sequence_first_i;
   int32_t sequence_last_i;
   char date_s [8];
} query_missing_trade_t;


/*  TRANSACTION TYPE = UQ103 */

typedef struct query_mm_exposure_limit_uq103
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_mm_exposure_limit_uq103_t;


/*  TRANSACTION TYPE = UQ101 */

typedef struct query_mm_parameters_uq101
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_mm_parameters_uq101_t;


/*   TRANSACTION TYPE = DQ18 */

typedef struct query_non_trading_days
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_non_trading_days_t;


/*   TRANSACTION TYPE = MQ3, MQ13, MQ23  */

typedef struct query_ob
{
   transaction_type_t transaction_type;
   series_t series;
} query_ob_t;


/*   TRANSACTION TYPE = MQ103  */

typedef struct query_ob_mq103
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_1_s;
   char filler_2_s [2];
   quad_word order_number_u;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} query_ob_mq103_t;


/*   TRANSACTION TYPE = IQ1, IQ257  */

typedef struct query_ob_volumes
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t block_n;
   char filler_2_s [2];
} query_ob_volumes_t;


/* TRANSACTION TYPE = CQ20, CQ276   */

typedef struct query_open_interest
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16_t segment_number_n;
   char date_s [8];
   char filler_2_s [2];
} query_open_interest_t;


/* TRANSACTION TYPE = CQ72, CQ328, CQ2120  */

typedef struct query_open_interest_ext
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16_t segment_number_n;
   char filler_2_s [2];
   char date_s [8];
} query_open_interest_ext_t;


/*  TRANSACTION TYPE = IQ111 */

typedef struct query_opra_prices_iq111
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16_t segment_number_n;
   uint8_t bin_c;
   char filler_1_s;
} query_opra_prices_iq111_t;


/*   TRANSACTION TYPE = MQ1, MQ11, MQ21, MQ81  */

typedef struct query_order
{
   transaction_type_t transaction_type;
   series_t series;
   whose_t whose;
} query_order_t;


/*   TRANSACTION TYPE = MQ101, MQ357  */

typedef struct query_order_mq101
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   uint8_t bin_c;
   uint8_t bid_or_ask_c;
   char filler_1_s;
   uint16_t order_category_n;
   uint16_t order_select_n;
   whose_t whose;
   quad_word order_number_u;
   filter_series_t filter_series;
} query_order_mq101_t;


/* TRANSACTION TYPE = UQ1 */

typedef struct query_partition
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_partition_t;


/*   TRANSACTION TYPE = MQ6  */

typedef struct query_party
{
   transaction_type_t transaction_type;
   series_t series;
} query_party_t;


/*  TRANSACTION TYPE = SQ1   */

typedef struct query_paynote_info
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
   char date_s [8];
} query_paynote_info_t;


/* TRANSACTION TYPE = VQ2, VQ258  */

typedef struct query_physical_delivery
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
   int32_t event_origin_i;
   char as_of_date_s [8];
} query_physical_delivery_t;


/*  TRANSACTION TYPE = UQ102 */

typedef struct query_pmm_parameters_uq102
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_pmm_parameters_uq102_t;


/*   TRANSACTION TYPE = MQ107  */

typedef struct query_pmm_price_imp_quote_mq107
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_3_s [3];
   quad_word order_number_u;
   uint8_t bid_or_ask_c;
   char filler_2_s [2];
   char filler_1_s;
} query_pmm_price_imp_quote_mq107_t;


/*   TRANSACTION TYPE = CQ3, CQ259, CQ2058  */

typedef struct query_position
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16_t segment_number_n;
   account_t account;
   char date_s [8];
   char filler_2_s [2];
} query_position_t;


/*   TRANSACTION TYPE = II17 */

typedef struct query_prel_settlement
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_prel_settlement_t;


/*  TRANSACTION TYPE = IQ101  */

typedef struct query_prices_iq101
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_3_s [3];
   uint16_t segment_number_n;
   uint8_t bin_c;
   char filler_1_s;
} query_prices_iq101_t;


/*  TRANSACTION TYPE = IQ102  */

typedef struct query_price_depth_iq102
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_3_s [3];
   uint16_t segment_number_n;
   uint8_t bin_c;
   char filler_1_s;
} query_price_depth_iq102_t;


/*   TRANSACTION TYPE = IQ2, IQ258  */

typedef struct query_price_info
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t block_n;
   char filler_2_s [2];
} query_price_info_t;


/*   TRANSACTION TYPE = MQ5, MQ15, MQ25, MQ47, MQ261, MQ281  */

typedef struct query_proxy_p
{
   transaction_type_t transaction_type;
   series_t series;
   whose_t whose;
   quad_word order_number_u;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} query_proxy_p_t;


/*   TRANSACTION TYPE = MQ104  */

typedef struct query_quote_mq104
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_3_s [3];
   whose_t whose;
   quad_word order_number_u;
   uint8_t bin_c;
   uint8_t bid_or_ask_c;
   char filler_2_s [2];
   filter_series_t filter_series;
} query_quote_mq104_t;


/* TRANSACTION TYPE = CQ16, CQ272  */

typedef struct query_rectify_d
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_rectify_d_t;


/* TRANSACTION TYPE = CQ17, CQ273   */

typedef struct query_rectify_d_cont
{
   transaction_type_t transaction_type;
   series_t series;
   int64_t rectify_deal_number_q;
} query_rectify_d_cont_t;


/* TRANSACTION TYPE = CQ14, CQ270   */

typedef struct query_rectify_t
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_rectify_t_t;


/* TRANSACTION TYPE = CQ15, CQ271   */

typedef struct query_rectify_t_cont
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t rectify_trade_number_i;
} query_rectify_t_cont_t;


/*   TRANSACTION TYPE = LQ2  */

typedef struct query_report
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   char filler_2_s [2];
   int32_t info_type_i;
} query_report_t;


/*   TRANSACTION TYPE = DQ2, DQ9, DQ102, DQ202, DQ152, DQ159 */

typedef struct query_series
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_series_t;


/*   TRANSACTION TYPE = DQ16 */

typedef struct query_series_delivery
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_series_delivery_t;


/*   TRANSACTION TYPE = DQ232 */

typedef struct query_series_dq232
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_series_dq232_t;


/* TRANSACTION TYPE = CQ31, CQ287  */

typedef struct query_simulate_fee
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t deal_price_i;
   int32_t deal_quantity_i;
   account_t account;
   uint8_t bid_or_ask_c;
   uint8_t open_close_req_c;
   char filler_2_s [2];
} query_simulate_fee_t;


/*   TRANSACTION TYPE = MQ4  */

typedef struct query_status
{
   transaction_type_t transaction_type;
   series_t series;
} query_status_t;


/* TRANSACTION TYPE = CQ49, CQ305   */

typedef struct query_tm_deal
{
   transaction_type_t transaction_type;
   series_t series;
   account_t account;
   char from_date_s [8];
   char to_date_s [8];
   char from_time_s [6];
   uint16_t segment_number_n;
   char to_time_s [6];
   char filler_2_s [2];
} query_tm_deal_t;


/*   TRANSACTION TYPE = IQ12  */

typedef struct query_tot_equil_prices
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_tot_equil_prices_t;


/*   TRANSACTION TYPE = MQ9, MQ19, MQ29, MQ49   */

typedef struct query_tot_inactive
{
   transaction_type_t transaction_type;
   series_t series;
   whose_t whose;
   quad_word order_number_u;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} query_tot_inactive_t;


/*   TRANSACTION TYPE = MQ305, MQ393, MQ403, MQ413, MQ433  */

typedef struct query_tot_inactive_p
{
   transaction_type_t transaction_type;
   series_t series;
   whose_t whose;
   quad_word order_number_u;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} query_tot_inactive_p_t;


/*   TRANSACTION TYPE = MQ7, MQ17, MQ27  */

typedef struct query_tot_ob
{
   transaction_type_t transaction_type;
   series_t series;
   quad_word order_number_u;
   uint8_t bid_or_ask_c;
   uint8_t only_this_series_c;
   char filler_2_s [2];
} query_tot_ob_t;


/*   TRANSACTION TYPE = MQ8, MQ18, MQ28, MQ48, MQ55, MQ58, MQ311  */

typedef struct query_tot_order
{
   transaction_type_t transaction_type;
   series_t series;
   whose_t whose;
   quad_word order_number_u;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} query_tot_order_t;


/*   TRANSACTION TYPE = MQ264, MQ392, MQ402, MQ412, MQ432  */

typedef struct query_tot_order_p
{
   transaction_type_t transaction_type;
   series_t series;
   whose_t whose;
   quad_word order_number_u;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} query_tot_order_p_t;


/*   TRANSACTION TYPE = MQ10  */

typedef struct query_tot_party
{
   transaction_type_t transaction_type;
   series_t series;
   quad_word order_number_u;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} query_tot_party_t;


/*   TRANSACTION TYPE = IQ10, IQ266  */

typedef struct query_tot_volume_prices
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_tot_volume_prices_t;


/*   TRANSACTION TYPE = IQ11, IQ267  */

typedef struct query_tot_volume_prices_bbo
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_tot_volume_prices_bbo_t;


/*   TRANSACTION TYPE = CQ2  */

typedef struct query_trade
{
   transaction_type_t transaction_type;
   series_t series;
   search_series_t search_series;
   uint16_t segment_number_n;
   char from_date_s [8];
   char from_time_s [6];
   char to_date_s [8];
   char to_time_s [6];
   char filler_2_s [2];
} query_trade_t;


/*   TRANSACTION TYPE = IQ45  */

typedef struct query_trade_deriv_history
{
   transaction_type_t transaction_type;
   series_t series;
   char from_date_s [8];
   char to_date_s [8];
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_trade_deriv_history_t;


/*   TRANSACTION TYPE = IQ42  */

typedef struct query_trade_statistics
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   char filler_2_s [2];
} query_trade_statistics_t;


/*   TRANSACTION TYPE = IQ44  */

typedef struct query_trade_uv_history
{
   transaction_type_t transaction_type;
   series_t series;
   char from_date_s [8];
   char to_date_s [8];
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_trade_uv_history_t;


/*   TRANSACTION TYPE = DQ29 */

typedef struct query_trading_state
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_trading_state_t;


/*   TRANSACTION TYPE = DQ245 */

typedef struct query_trading_state_dq245
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_trading_state_dq245_t;


/*   TRANSACTION TYPE = DQ4, DQ19, DQ104, DQ204, DQ154, DQ169 */

typedef struct query_underlying
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_underlying_t;


/*   TRANSACTION TYPE = DQ14  */

typedef struct query_underlying_adjustment
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_adjust_s [8];
   char filler_2_s [2];
} query_underlying_adjustment_t;


/*   TRANSACTION TYPE = II12, II268  */

typedef struct query_underlying_indices
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char date_s [8];
   char filler_2_s [2];
} query_underlying_indices_t;


/*  TRANSACTION TYPE = IQ121 */

typedef struct query_underlying_prices_iq121
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   uint8_t bin_c;
   char filler_1_s;
} query_underlying_prices_iq121_t;


/*  TRANSACTION TYPE = IQ132 */

typedef struct query_underlying_status_us
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t commodity_n;
   uint16_t segment_number_n;
} query_underlying_status_us_t;


/*  TRANSACTION TYPE = DQ30  */

typedef struct query_user_type_info
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_user_type_info_t;


/*   TRANSACTION TYPE = IQ4, IQ260  */

typedef struct query_volume_prices
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t block_n;
   char filler_2_s [2];
} query_volume_prices_t;


/*  TRANSACTION TYPE = DQ243  */

typedef struct query_vol_diss_step_dq243
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t segment_number_n;
   char filler_2_s [2];
} query_vol_diss_step_dq243_t;


/*  BROADCAST TYPE = BO108 */

typedef struct quote_rejection_reason_bo108
{
   broadcast_type_t broadcast_type;
   series_t series;
   quad_word order_number_u;
   int32_t premium_i;
   uint32_t reason_quote_rejected_u;
   uint16_t quantity_n;
   uint16_t open_quantity_n;
   uint8_t bid_or_ask_c;
   uint8_t item_number_c;
   char filler_2_s [2];
} quote_rejection_reason_bo108_t;


/*   TRANSACTION TYPE = MC1  */

typedef struct quote_request
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t block_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
   int32_t reserved_i;
} quote_request_t;


/*   BROADCAST TYPE = MI1  */

typedef struct quote_request_info
{
   broadcast_type_t broadcast_type;
   series_t series;
   uint16_t block_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
   int32_t reserved_i;
} quote_request_info_t;


/* TRANSACTION TYPE = MC4 */

typedef struct quote_request_vol
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t block_n;
   uint16_t mp_quantity_n;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} quote_request_vol_t;


/*   BROADCAST TYPE = MI4  */

typedef struct quote_request_vol_info
{
   broadcast_type_t broadcast_type;
   series_t series;
   uint16_t block_n;
   uint16_t mp_quantity_n;
   uint8_t bid_or_ask_c;
   char filler_3_s [3];
} quote_request_vol_info_t;


/*  BROADCAST TYPE = BO109 */

typedef struct quote_stepped_up_bo109
{
   broadcast_type_t broadcast_type;
   series_t series;
   quad_word order_number_u;
   int32_t premium_i;
   uint32_t quantity_u;
   uint16_t step_up_buffer_n;
   uint8_t bid_or_ask_c;
   char filler_1_s;
} quote_stepped_up_bo109_t;


/*  BROADCAST TYPE = BO110 */

typedef struct quote_tick_worse_bo110
{
   broadcast_type_t broadcast_type;
   series_t series;
   quad_word order_number_u;
   int32_t premium_i;
   uint32_t quantity_u;
   uint16_t isemm_trade_limit_absolute_n;
   uint16_t firm_trade_limit_absolute_n;
   uint16_t farmm_trade_limit_absolute_n;
   uint16_t step_up_buffer_n;
   uint16_t tick_worse_level_n;
   uint8_t bid_or_ask_c;
   uint8_t reason_tick_worse_c;
} quote_tick_worse_bo110_t;


/*  TRANSACTION TYPE = MO106  */

typedef struct quote_trans_mo106
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_1_s;
   uint16_t items_n;
   quote_trans_mo106_item_t item [192];
} quote_trans_mo106_t;


/*   TRANSACTION TYPE = CD31, CD287  */

typedef struct rectify_deal
{
   transaction_type_t transaction_type;
   series_t series;
   other_series_t other_series;
   int32_t deal_price_i;
   int32_t deal_number_i;
   uint8_t operation_c;
   char filler_3_s [3];
   uint16_t items_n;
   char filler_2_s [2];
   rectify_deal_item_t item [255];
} rectify_deal_t;


/* TRANSACTION TYPE = CD27, CD28, CD127, CD283, CD284, CD383, CD384  */

typedef struct rectify_trade
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t trade_number_i;
   uint8_t items_c;
   char filler_3_s [3];
   rectify_trade_item_t item [100];
} rectify_trade_t;


/* TRANSACTION TYPE = CD36, CD292  */

typedef struct rectify_trade_ise
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t trade_number_i;
   uint8_t items_c;
   char filler_3_s [3];
   rectify_trade_ise_item_t item [100];
} rectify_trade_ise_t;


/*   TRANSACTION TYPE = CD33, CD289  */

typedef struct rectify_trade_occ_us
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t trade_number_i;
   char cmta_member_s [5];
   char sub_account_id_s [5];
   char customer_info_s [15];
   char account_type_c;
   uint8_t open_close_req_c;
   char filler_1_s;
} rectify_trade_occ_us_t;


/*  TRANSACTION TYPE = CC21, CC277, VC1, VC257  */

typedef struct reg_physical_delivery
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t event_origin_i;
   int32_t class_no_i;
   int32_t sequence_no_i;
   uint16_t items_n;
   char filler_2_s [2];
   reg_physical_delivery_item_t item [20];
} reg_physical_delivery_t;


/*  TRANSACTION TYPE = CC40  */

typedef struct reject_give_up_request
{
   transaction_type_t transaction_type;
   series_t series;
   int32_t give_up_number_i;
   char give_up_text_s [30];
   char filler_2_s [2];
} reject_give_up_request_t;


/*   TRANSACTION TYPE = BC515  */

typedef struct remove_tm_combo
{
   broadcast_type_t broadcast_type;
   series_t series;
} remove_tm_combo_t;


/*  TRANSACTION TYPE = MO113, MQ114 */

typedef struct rotation_request_mo113_mq114
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   uint8_t series_status_c;
   uint16_t fast_market_level_n;
   char filler_2_s [2];
   uint16_t items_n;
   rotation_request_mo113_mq114_item_t item [100];
} rotation_request_mo113_mq114_t;


/*   BROADCAST TYPE = BU9 */

typedef struct series_bo_update_bu9
{
   broadcast_type_t broadcast_type;
   uint16_t chg_type_n;
   char filler_2_s [2];
   da9_t da9;
} series_bo_update_bu9_t;


/*  BROADCAST TYPE = BI103 */

typedef struct series_status_change_bi103
{
   broadcast_type_t broadcast_type;
   uint16_t items_n;
   char filler_2_s [2];
   series_status_change_bi103_item_t item [50];
} series_status_change_bi103_t;


/*   BROADCAST TYPE = BI202 */

typedef struct series_update_bi202
{
   broadcast_type_t broadcast_type;
   uint16_t chg_type_n;
   char filler_2_s [2];
   da202_t da202;
} series_update_bi202_t;


/*   BROADCAST TYPE = BU2 */

typedef struct series_update_bu2
{
   broadcast_type_t broadcast_type;
   uint16_t chg_type_n;
   char filler_2_s [2];
   da2_t da2;
} series_update_bu2_t;


/*   BROADCAST TYPE = BI63  */

typedef struct settle_price_update
{
   broadcast_type_t broadcast_type;
   uint16_t items_n;
   char filler_2_s [2];
   settle_price_update_item_t item [50];
} settle_price_update_t;


/* TRANSACTION TYPE = CC14, CC270   */

typedef struct set_deny_exercise
{
   transaction_type_t transaction_type;
   series_t series;
   account_t account;
   int64_t deny_exercise_q;
} set_deny_exercise_t;


/*   TRANSACTION TYPE = UI103 */

typedef struct set_mm_exposure_limit_ui103
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t items_n;
   char filler_2_s [2];
   set_mm_exposure_limit_ui103_item_t item [1000];
} set_mm_exposure_limit_ui103_t;


/*  TRANSACTION TYPE = UI101 */

typedef struct set_mm_parameters_ui101
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t items_n;
   char filler_2_s [2];
   set_mm_parameters_ui101_item_t item [900];
} set_mm_parameters_ui101_t;


/*   TRANSACTION TYPE = UI102 */

typedef struct set_pmm_parameters_ui102
{
   transaction_type_t transaction_type;
   series_t series;
   uint16_t items_n;
   char filler_2_s [2];
   set_pmm_parameters_ui102_item_t item [400];
} set_pmm_parameters_ui102_t;


/*  BROADCAST TYPE = BO106 */

typedef struct special_order_bo106
{
   broadcast_type_t broadcast_type;
   series_t series;
   quad_word order_number_u;
   int32_t premium_i;
   uint32_t quantity_u;
   uint16_t expiration_time_n;
   uint16_t time_validity_n;
   uint16_t order_category_n;
   uint8_t expose_flag_c;
   uint8_t broker_percentage_c;
   trading_code_t trading_code;
   uint8_t client_category_c;
   uint8_t bid_or_ask_c;
   uint8_t client_category_counter_party_c;
   uint8_t bid_or_ask_counter_party_c;
} special_order_bo106_t;


/*  BROADCAST TYPE = BO112 */

typedef struct special_order_ended_bo112
{
   broadcast_type_t broadcast_type;
   series_t series;
   quad_word order_number_u;
   uint16_t order_category_n;
   char filler_2_s [2];
} special_order_ended_bo112_t;


/*  TRANSACTION TYPE = MO109 */

typedef struct special_order_response_mo109
{
   transaction_type_t transaction_type;
   series_t series;
   uint8_t orderbook_c;
   char filler_3_s [3];
   quad_word order_number_u;
   int32_t premium_i;
   uint32_t quantity_u;
   uint16_t order_category_n;
   uint8_t bid_or_ask_c;
   uint8_t open_close_req_c;
   char ex_client_s [10];
   char broker_id_s [5];
   char free_text_s [15];
   give_up_member_t give_up_member;
   uint8_t client_category_c;
   uint8_t demand_quantity_c;
} special_order_response_mo109_t;


/*  BROADCAST TYPE = BI12, BI268 */

typedef struct state_change
{
   broadcast_type_t broadcast_type;
   series_t series;
   int32_t ext_status_i;
   int32_t ext_state_i;
   int32_t ext_previous_state_i;
   int32_t trade_number_i;
   uint8_t open_close_c;
   char filler_3_s [3];
   int32_t reserved_1_i;
} state_change_t;


/*   TRANSACTION TYPE = MO297, MO425  */

typedef struct stop_order_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   order_var_t order_var;
   stop_series_t stop_series;
   int32_t limit_premium_i;
   uint16_t total_volume_n;
   char filler_2_s [2];
} stop_order_trans_p_t;


/* TRANSACTION TYPE = BD6, BO14   */

typedef struct sub_item_hdr
{
   uint16_t named_struct_n;
   uint16_t size_n;
} sub_item_hdr_t;


/*   BROADCAST TYPE = BI1  */

typedef struct suspend_resume_trading
{
   broadcast_type_t broadcast_type;
   uint16_t commodity_n;
   uint8_t on_off_c;
   char filler_1_s;
} suspend_resume_trading_t;


/*   TRANSACTION TYPE = MO55  */

typedef struct tm_trade_rpt_trans
{
   transaction_type_t transaction_type;
   series_t series;
   order_var_t order_var;
   party_t party;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} tm_trade_rpt_trans_t;


/*   TRANSACTION TYPE = MO311, MO439  */

typedef struct tm_trade_rpt_trans_p
{
   transaction_type_t transaction_type;
   series_t series;
   trading_code_t trading_code;
   order_var_t order_var;
   party_t party;
   uint16_t commodity_n;
   uint16_t expiration_date_n;
   int32_t strike_price_i;
} tm_trade_rpt_trans_p_t;


/*  BROADCAST TYPE = BD101 */

typedef struct trade_info_bd101
{
   broadcast_type_t broadcast_type;
   series_t series;
   int32_t closing_price_i;
   int32_t opening_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   uint32_t volume_u;
   uint32_t turnover_u;
   uint32_t open_balance_u;
   uint8_t out_of_sequence_c;
   char filler_3_s [3];
   int32_t reserved_i;
} trade_info_bd101_t;


/*  BROADCAST TYPE = BI40 */

typedef struct trade_state_ext
{
   broadcast_type_t broadcast_type;
   cl_trade_api_t cl_trade_api;
   int32_t ext_status_i;
   uint32_t open_balance_u;
   int64_t nbr_held_q;
   int64_t nbr_written_q;
} trade_state_ext_t;


/*  TRANSACTION TYPE = UC51 */

typedef struct trans_hdr
{
   transaction_type_t transaction_type;
   uint16_t items_n;
   uint16_t size_n;
} trans_hdr_t;


/*  BROADCAST TYPE = BO105 */

typedef struct underlying_best_bid_offer_bo105
{
   broadcast_type_t broadcast_type;
   series_t series;
   int32_t best_ask_premium_i;
   int32_t best_bid_premium_i;
   uint8_t nasdaq_upc_indicator_c;
   uint8_t nasdaq_short_sale_bid_tick_c;
   uint8_t originator_id_c;
   char reserved_8_s [8];
   char filler_1_s;
} underlying_best_bid_offer_bo105_t;


/*   BROADCAST TYPE = BD3, BD259 */

typedef struct underlying_info
{
   broadcast_type_t broadcast_type;
   int32_t bid_premium_i;
   int32_t ask_premium_i;
   int32_t closing_price_i;
   int32_t opening_price_i;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   int32_t ref_price_i;
   uint32_t turnover_u;
   uint16_t commodity_n;
   char ext_time_s [6];
} underlying_info_t;


/*  BROADCAST TYPE = BD103 */

typedef struct underlying_trade_info_bd103
{
   broadcast_type_t broadcast_type;
   series_t series;
   int32_t high_price_i;
   int32_t low_price_i;
   int32_t last_price_i;
   uint32_t volume_u;
   uint32_t turnover_u;
   char reserved_8_s [8];
} underlying_trade_info_bd103_t;


/*  BROADCAST TYPE = BI132 */

typedef struct underlying_trading_status_bi132
{
   broadcast_type_t broadcast_type;
   uint16_t commodity_n;
   uint8_t underlying_trading_status_c;
   char filler_1_s;
   char reserved_8_s [8];
} underlying_trading_status_bi132_t;


/*   BROADCAST TYPE = BI204 */

typedef struct underlying_update_bi204
{
   broadcast_type_t broadcast_type;
   uint16_t chg_type_n;
   char filler_2_s [2];
   da204_t da204;
} underlying_update_bi204_t;


/*   BROADCAST TYPE = BU4, BU19 */

typedef struct underlying_update_bu4_bu19
{
   broadcast_type_t broadcast_type;
   uint16_t chg_type_n;
   char filler_2_s [2];
   da4_da19_t da4_da19;
} underlying_update_bu4_bu19_t;





#ifndef _CDB_MESSAGES_H_
#define _CDB_MESSAGES_H_

/******************************************************************************
Module: cdb_messages.h

Message definitions generated 2001-10-24 17:29:11 by amt, version 3.1.0.0.

NOTE: Do not edit this file. It is automatically generated from
      one or more message files.
*******************************************************************************/

#define CDB_SUCCCOMP                    300001
#define CDB_NORMAL                      300003
#define CDB_NOMAREXIST                 -300005
#define CDB_NOCOUEXIST                 -300007
#define CDB_NOMSUEXIST                 -300009
#define CDB_INSFMEM                    -300010
#define CDB_SQLDECLERR                 -300012
#define CDB_SQLERROR                   -300014
#define CDB_UNTRTNO                    -300016
#define CDB_ILLEXPYEAR                 -300018
#define CDB_NOTBLMATCH                 -300020
#define CDB_USRNOTEXIST                -300022
#define CDB_WRONGACCTYPE               -300024
#define CDB_NOVALIDUST                 -300026
#define CDB_NOVALIDUSR                 -300028
#define CDB_USRNOTLEG                  -300030
#define CDB_SECMISMTC                  -300032
#define CDB_USER_TIMEOUT               -300034
#define CDB_USER_LOST                  -300036
#define CDB_OLDTIMESEC                 -300038
#define CDB_MNUNOTEXIST                -300040
#define CDB_ILLEXPMONTH                -300042
#define CDB_DBRETRY                    -300044
#define CDB_VECFULL                    -300046
#define CDB_BUGCHECK                   -300048
#define CDB_NOICOEXIST                  300049
#define CDB_NOITUEXIST                  300051
#define CDB_NOLTFEXIST                  300053
#define CDB_NOPQREXIST                  300055
#define CDB_NOTRFEXIST                  300057
#define CDB_NOVMSEXIST                  300059
#define CDB_NOVOLEXIST                  300061
#define CDB_NOPSCEXIST                  300063
#define CDB_NOCOPEXIST                  300065
#define CDB_NOCBSEXIST                 -300067
#define CDB_NOCOMEXIST                 -300069
#define CDB_NOIDBEXIST                 -300071
#define CDB_NOIDLEXIST                 -300073
#define CDB_NOINSEXIST                 -300075
#define CDB_NOINTEXIST                 -300077
#define CDB_NOLITEXIST                 -300079
#define CDB_NOLTTEXIST                 -300081
#define CDB_NOLTUEXIST                 -300083
#define CDB_NOTRLEXIST                 -300085
#define CDB_NOTRSEXIST                 -300087
#define CDB_NOTSFEXIST                 -300089
#define CDB_NOTRTEXIST                 -300091
#define CDB_NOTZOEXIST                 -300093
#define CDB_NOUSREXIST                 -300095
#define CDB_NOUSTEXIST                 -300097
#define CDB_NOVBSEXIST                 -300099
#define CDB_NOVPTEXIST                 -300101
#define CDB_NOMNUACCESS                -300103
#define CDB_USRNOTLOG                  -300105
#define CDB_NOPREVINA                  -300107
#define CDB_NOCLHEXIST                 -300109
#define CDB_NOMEMEXIST                 -300111
#define CDB_NOINCEXIST                 -300113
#define CDB_NOCLIEXIST                 -300115
#define CDB_NOBBOEXIST                 -300117
#define CDB_NOCRSEXIST                 -300119
#define CDB_NOSPREXIST                 -300121
#define CDB_NOTRCEXIST                 -300123
#define CDB_NOTRREXIST                 -300125
#define CDB_NOOBLEXIST                 -300127
#define CDB_NOLFAEXIST                 -300129
#define CDB_NONODEXIST                 -300131
#define CDB_NOLNOEXIST                 -300133
#define CDB_NOISSEXIST                  300135
#define CDB_NOCSSEXIST                  300137
#define CDB_NOPCOEXIST                  300139
#define CDB_NOPICEXIST                  300141
#define CDB_NOPISEXIST                  300143
#define CDB_NOPRDEXIST                  300145
#define CDB_NOPSREXIST                  300147
#define CDB_NOPUREXIST                  300149
#define CDB_NOUCLEXIST                  300151
#define CDB_NOWINEXIST                  300153
#define CDB_NOVPDEXIST                  300155
#define CDB_NOISDEXIST                  300157
#define CDB_NOISCEXIST                  300159
#define CDB_NOCOGEXIST                  300161
#define CDB_NOFMLEXIST                  300163
#define CDB_NOSYIEXIST                  300165
#define CDB_NOOBKEXIST                  300167
#define CDB_NOTSREXIST                  300169
#define CDB_NOEDGEXIST                  300171
#define CDB_NOSPGEXIST                  300173
#define CDB_NOPDPEXIST                  300175
#define CDB_NOFOVEXIST                  300177
#define CDB_NOCLPEXIST                  300179
#define CDB_NOCLHUSR                   -300181
#define CDB_NOAFTEXIST                 -300183
#define CDB_NOATYEXIST                 -300185
#define CDB_NOADCEXIST                 -300187
#define CDB_NOCSBEXIST                 -300189
#define CDB_NOEXCEXIST                 -300191
#define CDB_NOFASEXIST                 -300193
#define CDB_NOFTBEXIST                 -300195
#define CDB_NONTDEXIST                 -300197
#define CDB_NOPEVEXIST                 -300199
#define CDB_NOPFTEXIST                 -300201
#define CDB_NOTRUEXIST                 -300203
#define CDB_INVCOMMAND                 -300205
#define CDB_NOSYMEXIST                 -300207
#define CDB_NOACCEXIST                 -300209
#define CDB_NOSBSEXIST                 -300211
#define CDB_WRONGWEEK                  -300212
#define CDB_SLEC_INSERR                -300214
#define CDB_SLEC_EXCMISSING            -300216
#define CDB_SLEC_DIFFCODE              -300218
#define CDB_SLEC_DIFFISIN              -300220
#define CDB_NODLVEXIST                 -300221
#define CDB_NOCOMSPEC                  -300222
#define CDB_ERRLONGCOM                 -300224
#define CDB_NOMARSPEC                  -300226
#define CDB_ERRLONGMAR                 -300228
#define CDB_NOINGSPEC                  -300230
#define CDB_ERRLONGING                 -300232
#define CDB_ERRLONGINS                 -300234
#define CDB_NOSTRKSPEC                 -300236
#define CDB_ERRLONGSTRK                -300238
#define CDB_NOSTRKDECSPEC              -300240
#define CDB_ERRLONGSTRKDEC             -300242
#define CDB_NOCONTRSPEC                -300244
#define CDB_ERRLONGCONTR               -300246
#define CDB_NOCONTRDECSPEC             -300248
#define CDB_ERRLONGCONTRDEC            -300250
#define CDB_NOEXPSPEC                  -300252
#define CDB_ERRLONGEXP                 -300254
#define CDB_ERRLONGLTD                 -300256
#define CDB_ERRLONGLTT                 -300258
#define CDB_ERRLONGISIN                -300260
#define CDB_ERRLONGIMPLD               -300262
#define CDB_ERRLONGIMPLT               -300264
#define CDB_NOOPERSPEC                 -300266
#define CDB_ERRLONGOPER                -300268
#define CDB_ERRLONGDLVSTART            -300270
#define CDB_ERRLONGDLVSTOP             -300272
#define CDB_ERRLONGSTART               -300274
#define CDB_ERRLONGEND                 -300276
#define CDB_ERRLONGSYMB                -300278
#define CDB_ERRLONGMTH                 -300280
#define CDB_ERRLONGSTR                 -300282
#define CDB_ERRLONGEXC                 -300284
#define CDB_ERRLONGSTATUS              -300286
#define CDB_ERRLONGLISTING             -300288
#define CDB_WRONGINPUT                  300289
#define CDB_NOSERIES                    300291
#define CDB_NOINSTRUMENT                300293
#define CDB_NOUNDERLYING                300295
#define CDB_NOCOMBO                     300297
#define CDB_NOUSER                      300299
#define CDB_MARCLOSED                   300301
#define CDB_COUCLOSED                   300303
#define CDB_NOTRM                       300305
#define CDB_NOMARKET                    300307
#define CDB_NOINSTGROUP                 300309
#define CDB_UNEXPECTED                 -300310
#define CDB_WRONGSEGM                  -300312
#define CDB_NOINSTCLASS                 300313
#define CDB_NOBASEDFOUND               -300314
#define CDB_NOACCOUNT                   300315
#define CDB_NOSYMBOL                    300317
#define CDB_WRONGEXP                   -300318
#define CDB_ILLEGTRD                   -300320
#define CDB_WRONGCOM                   -300322
#define CDB_DUMMY                      -300324
#define CDB_RTRERROR                   -300326
#define CDB_ABORT                      -300328
#define CDB_ILLCHANNEL                 -300330
#define CDB_NOSRVAVAIL                 -300332
#define CDB_INSERTFAILED               -300334
#define CDB_REMOVEFAILED               -300336
#define CDB_NOTFOUNDDB                 -300338
#define CDB_NOTFOUNDMEM                -300340
#define CDB_UCLEXCEED                  -300342
#define CDB_NOTAREPO                   -300344
#define CDB_NOTMPLFOUND                -300346
#define CDB_STARTWRONG                 -300348
#define CDB_STARTNOTSETTL              -300350
#define CDB_ENDWRONG                   -300352
#define CDB_ENDNOTSETTL                -300354
#define CDB_WRONGMONEYPAR              -300356
#define CDB_WRONGNOOFSUB               -300358
#define CDB_WRONGBUYSELLBACK           -300360
#define CDB_NOINCFOUND                 -300362
#define CDB_REPOALREXIST                300363
#define CDB_REPOTOOLONG                -300364
#define CDB_LOADFAILED                 -300366
#define CDB_STARTNOTVALID              -300368
#define CDB_ENDNOTVALID                -300370
#define CDB_OUTRSPECMISSING            -300372
#define CDB_STARTBEFISSUE              -300374
#define CDB_ENDAFTERMATUR              -300376
#define CDB_SERIESALREXIST              300377
#define CDB_TMNOTALLOWED               -300378
#define CDB_INSEMPTY                   -300380
#define CDB_NOCTDPRICE                 -300382
#define CDB_INSNOTFOUND                -300384
#define CDB_ENDAFTEREQMATUR            -300386
#define CDB_NOTENOUGHLEGS              -300388
#define CDB_COMBONOTTM                 -300390
#define CDB_NOCONVFACTOR               -300392
#define CDB_ADDISIN                     300393
#define CDB_STARTTOEARLY               -300394
#define CDB_ENDTOEARLY                 -300396
#define PD_ACTIVE                       305001
#define PD_EXPISNTD                    -305002
#define PD_LTRISNTD                     305003
#define PD_ALREADYRESUMED               305005
#define PD_ALREADYSUSP                  305007
#define PD_ALREADYEXIST                 305009
#define PD_NOTEXIST                     305011
#define PD_SGNTXSTART                   305013
#define PD_BUFOVFLOW                   -305015
#define PD_UNEXPECTED                  -305016
#define PD_HIGHISINNOTCORR             -305018
#define PD_LOWISINNOTCORR              -305020
#define PD_LOWERISHIGHER               -305022
#define PD_ISINOLAP                    -305024
#define PD_ISINMISS                    -305026
#define PD_SEGMZERO                    -305028
#define PD_INVONOFF                    -305030
#define PD_SERCODEEMPTY                -305032
#define PD_INGCODEZERO                 -305034
#define PD_MARCODEZERO                 -305036
#define PD_OPERNOTVALID                -305038
#define PD_DELNOTVALID                 -305040
#define PD_ISINNOTUNIQUE               -305042
#define PD_WRONGIMPLFORMAT             -305044
#define PD_WRONGEXPFORMAT              -305046
#define PD_WRONGLASTFORMAT             -305048
#define PD_ORIGSERNOTFOUND             -305050
#define PD_UPPSERNOTFOUND              -305052
#define PD_EXTSERIESTRT                -305054
#define PD_NOTACTIVE                   -305056
#define PD_NOTFOUND                    -305058
#define PD_FUTDELETE                   -305060
#define PD_NOTAUTHORIZED               -305062
#define PD_ILLKEYVALUE                 -305064
#define PD_SPACESINKEY                 -305066
#define PD_ILLSEARCHVALUE              -305068
#define PD_FIELDISEMPTY                -305070
#define PD_COMCODEALREXIST             -305072
#define PD_COMIDALREXIST               -305074
#define PD_INGCODEALREXIST             -305076
#define PD_INGIDALREXIST               -305078
#define PD_INTCODEALREXIST             -305080
#define PD_INTIDALREXIST               -305082
#define PD_INCCODEALREXIST             -305084
#define PD_INCIDALREXIST               -305086
#define PD_INSCODEALREXIST             -305088
#define PD_INSIDALREXIST               -305090
#define PD_MARCODEALREXIST             -305092
#define PD_MARIDALREXIST               -305094
#define PD_EXCCODEALREXIST             -305096
#define PD_EXCIDALREXIST               -305098
#define PD_CBGCODEALREXIST             -305100
#define PD_CBGIDALREXIST               -305102
#define PD_CBTCODEALREXIST             -305104
#define PD_CBTIDALREXIST               -305106
#define PD_CBCCODEALREXIST             -305108
#define PD_CBCIDALREXIST               -305110
#define PD_CBSCODEALREXIST             -305112
#define PD_CBSIDALREXIST               -305114
#define PD_WRONGPERIOD                 -305116
#define PD_EXPNOTLEG                   -305118
#define PD_DELNOTFOUND                 -305120
#define PD_TIMEISNULL                  -305122
#define PD_NOTEXACT                    -305124
#define PD_TIMEISPASSED                -305126
#define PD_INCNOTTRADED                -305128
#define PD_WRONGISIN                   -305130
#define PD_COMILLSTATUS                -305132
#define PD_ILLPASSW                    -305134
#define PD_INSILLSTATUS                -305136
#define PD_EXPISPASSED                 -305138
#define PD_STRIKEISZERO                -305140
#define PD_STRIKENOTZERO               -305142
#define PD_IDNOTCREATED                -305144
#define PD_OPRANOTUNIQUE               -305146
#define PD_OPRASTRKNOTUNIQUE           -305148
#define PD_REMOVENOTALLWD              -305150
#define PD_FIELDEXCEEDED               -305152
#define PD_DOUBLEMENU                  -305154
#define PD_DOUBLEMENUTXT               -305156
#define PD_MNUNOTCHGABLE               -305158
#define PD_USRNOTFOUND                 -305160
#define PD_ACTNOTFOUND                 -305162
#define PD_MTXNOTFOUND                 -305164
#define PD_MACNOTFOUND                 -305166
#define PD_MNUNOTFOUND                 -305168
#define PD_BONDPARAM                   -305170
#define PD_POWPARAM                    -305172
#define PD_POWSTARTGTSTOP              -305174
#define PD_DOUBLECUPDATE               -305176
#define PD_BOTHWINSUM                  -305178
#define PD_SUMAFTERWIN                 -305180
#define PD_STOPAFTERREST               -305182
#define PD_DOUBLECOM                   -305184
#define PD_DOUBLESUMDATE               -305186
#define PD_DOUBLEWINDATE               -305188
#define PD_DOUBLESTOPDATE              -305190
#define PD_DOUBLERESTDATE              -305192
#define PD_DOUBLEINS                   -305194
#define PD_DOUBLESIZE                  -305196
#define PD_DOUBLEMODIFIER              -305198
#define PD_DOUBLESPREAD                -305200
#define PD_DOUBLELOWER                 -305202
#define PD_DOUBLEUPPER                 -305204
#define PD_LOWERGEUPPER                -305206
#define PD_LOWEROVRLAP                 -305208
#define PD_UPPEROVRLAP                 -305210
#define PD_DOUBLEMONTH                 -305212
#define PD_DOUBLEPERIOD                -305214
#define PD_DOUBLEORDERNO               -305216
#define PD_ILLEGMONTH                  -305218
#define PD_ILLEGPERIOD                 -305220
#define PD_DOUBLEEXP                   -305222
#define PD_WRONGEXP                    -305224
#define PD_WRONGNOT                    -305226
#define PD_WRONGLIFETIME               -305228
#define PD_DOUBLELEG                   -305230
#define PD_SERIEISPASSED               -305232
#define PD_LTDISPASSED                 -305234
#define PD_TOOFEWLEGS                  -305236
#define PD_CBCNOTTRADED                -305238
#define PD_DOUBLEINC                   -305240
#define PD_DOUBLEWIN                   -305242
#define PD_DOUBLEPEV                   -305244
#define PD_SERIESNOTEXIST              -305246
#define PD_DISCEXCEED                  -305248
#define PD_DOUBLEFVL                   -305250
#define PD_FVLNOTEXIST                 -305252
#define PD_DOUBLEFTB                   -305254
#define PD_FEEVALEXCEED                -305256
#define PD_LIMITSEARCH                 -305258
#define PD_ILLEGMONTHNAME              -305260
#define PD_ILLEGPERIODNAME             -305262
#define PD_DOUBLELGP                   -305264
#define PD_DOUBLETRU                   -305266
#define PD_WROPEN                      -305268
#define PD_WRNET                       -305270
#define PD_DOUBLECHA                   -305272
#define PD_DOUBLECLI                   -305274
#define PD_EXCWCNOTALLWD               -305276
#define PD_DOUBLESPR                   -305278
#define PD_SPRCHKLEG                   -305280
#define PD_TRRCODEEMPTY                -305282
#define PD_CONDEMPTY                   -305284
#define PD_SPRCHKEMPTY                 -305286
#define PD_PUBINFEMPTY                 -305288
#define PD_DOUBLETRC                   -305290
#define PD_DOUBLEPRDDATE               -305292
#define PD_ACCNUMALREXIST              -305294
#define PD_DOUBLETRT                   -305296
#define PD_WRACTTIME                   -305298
#define PD_WRCONTROP                   -305300
#define PD_WRCONTRFACSUB               -305302
#define PD_WRCONTRFAC                  -305304
#define PD_DOUBLEISD                   -305306
#define PD_ISDNOTEQ                    -305308
#define PD_WRSERIES                    -305310
#define PD_DOUBLEINSID                 -305312
#define PD_DOUBLESOINSID               -305314
#define PD_OLDINSNOTEXIST              -305316
#define PD_CODEISEQ                    -305318
#define PD_SOCODEISEQ                  -305320
#define PD_INSINPREV                   -305322
#define PD_PASSEDDATE                  -305324
#define PD_ALREXIST                    -305326
#define PD_NOCHGALW                    -305328
#define PD_EXCEEDCONTR                 -305330
#define PD_NAMEISEQ                    -305332
#define PD_SOOLDNAMEISEQ               -305334
#define PD_SONEWNAMEISEQ               -305336
#define PD_WRINFSRC                    -305338
#define PD_INFSRCNACT                  -305340
#define PD_WRINFTYPE                   -305342
#define PD_WRONGATTR                   -305344
#define PD_WRONGGENATTR                -305346
#define PD_WRONGDERATTR                -305348
#define PD_BOTHATTR                    -305350
#define PD_BOTHCOMEXP                  -305352
#define PD_ISSALREXIST                 -305354
#define PD_ISSNOSERIES                 -305356
#define PD_DOUBLEMNA                   -305358
#define PD_DOUBLEINT                   -305360
#define PD_DOUBLECBT                   -305362
#define PD_FROMTOP                     -305364
#define PD_FROMTOR                     -305366
#define PD_FROMTOI                     -305368
#define PD_FRPBLANK                    -305370
#define PD_FRPNOTBLANK                 -305372
#define PD_FRRBLANK                    -305374
#define PD_FRRNOTBLANK                 -305376
#define PD_WHPBLANK                    -305378
#define PD_WHPNOTBLANK                 -305380
#define PD_WHRBLANK                    -305382
#define PD_WHRNOTBLANK                 -305384
#define PD_OTHQBLANK                   -305386
#define PD_OTHBNOTBLANK                -305388
#define PD_OTHBBLANK                   -305390
#define PD_OTHQNOTBLANK                -305392
#define PD_DOUBLPFT                    -305394
#define PD_BUYSELL                     -305396
#define PD_RESP                        -305398
#define PD_ISNNODIG                    -305400
#define PD_MAXISIN                     -305402
#define PD_WRONGCBA                    -305404
#define PD_NODLVFOUND                  -305406
#define PD_NODLVEXPFOUND               -305408
#define PD_IFILL                       -305410
#define PD_INSDUP                      -305412
#define PD_NOSTORE                     -305414
#define PD_SGNTOMGRP                   -305416
#define PD_SGNTOMSER                   -305418
#define PD_SGNTOMNEWSER                -305420
#define PD_SGNTOMSTRK                  -305422
#define PD_SGNUNDZERO                  -305424
#define PD_SGNONGTX                    -305426
#define PD_EXPINVDAY                   -305428
#define PD_EXPINVWEEK                  -305430
#define PD_EXPINVMTH                   -305432
#define PD_ISNCODEALREXIST             -305434
#define PD_ISNIDALREXIST               -305436
#define PD_TRRCODEALREXIST             -305438
#define PD_TRRIDALREXIST               -305440
#define PD_COMCODEZERO                 -305442
#define PD_DEFMUSTBEOPEN               -305444
#define PD_MUSTBEOPEN                  -305446
#define PD_NOFLTITEM                   -305448
#define PD_DOUBLEQUA                   -305450
#define PD_DOUBLEBAS                   -305452
#define PD_DOUBLEOTHBAS                -305454
#define PD_DOUBLEOTHQUA                -305456
#define PD_NOPVTITEM                   -305458
#define PD_NOLITITEM                   -305460
#define PD_CBTANDINT                   -305462
#define PD_NOMACITEM                   -305464
#define PD_NOMNUITEM                   -305466
#define PD_DOUBLESBS                   -305468
#define PD_ERRFROMATY                  -305470
#define PD_DOUBLEPERCODE               -305472
#define PD_DOUBLEPERNAME               -305474
#define PD_ILLEGDATE                   -305476
#define PD_LTTSTATUSUNL                -305478
#define PD_LTTSTARTTIMEEMPTY           -305480
#define PD_LTTSTARTTIMENOTEMPTY        -305482
#define PD_LTTSTOPTIMEEMPTY            -305484
#define PD_LTTSTOPTIMENOTEMPTY         -305486
#define PD_NOISSSERIES                 -305488
#define PD_INCANDINS                   -305490
#define PD_NOEXPINS                    -305492
#define PD_INVPQREXP                   -305494
#define PD_VOLLSTEMPTY                 -305496
#define PD_HLSTEMPTY                   -305498
#define PD_HDLMEMPTY                   -305500
#define PD_TIMBEFALEMPTY               -305502
#define PD_TIMBETWALEMPTY              -305504
#define PD_CSTNOTALLWD                 -305506
#define PD_VOLANDLST                   -305508
#define PD_WRONGCONTR                  -305510
#define PD_NOPFTITEM                   -305512
#define PD_ALLFEEVALEMPTY              -305514
#define PD_PQRDELNOTVALID              -305516
#define PD_UNDALREADYRESUMED           -305518
#define PD_UNDALREADYSUSP              -305520
#define PD_CSTALREADYRESUMED           -305522
#define PD_CSTALREADYSUSP              -305524
#define PD_WACCNOTALLOWED              -305526
#define PD_INSDEL                      -305528
#define PD_EXPWEEKEMPTY                -305530
#define PD_EXPMTHEMPTY                 -305532
#define PD_CBRSTPEXCEED                -305534
#define PD_CBRINCSTEPS                 -305536
#define PD_NOCLIITEM                   -305538
#define PD_INSISDELETED                -305540
#define PD_OBLDOUBLE                   -305542
#define PD_OBLRWDOUBLE                 -305544
#define PD_ISINCODEEMPTY               -305546
#define PD_DOUBLENOD                   -305548
#define PD_DOUBLEFAC                   -305550
#define PD_USERLOCK                    -305552
#define PD_SLECCOMINCOMPL              -305554
#define PD_ISINTSYSEMPTY               -305556
#define PD_MODIFIEREMPTY               -305558
#define PD_ISSIDENTEMPTY               -305560
#define PD_COGCODEALREXIST             -305562
#define PD_COGIDALREXIST               -305564
#define PD_OBKCODEALREXIST             -305566
#define PD_OBKIDALREXIST               -305568
#define PD_DOUBLEFML                   -305570
#define PD_POSORTRDEXIST               -305572
#define PD_NOCLAVAIL                   -305574
#define PD_PMMANDCGM                   -305576
#define PD_DOUBLECGM                   -305578
#define PD_GUARDPRICEEMPTY             -305580
#define PD_VALINTTYPEEMPTY             -305582
#define PD_VALINTBASEEMPTY             -305584
#define PD_GROSSMARGPROPEMPTY          -305586
#define PD_DELIVPROPEMPTY              -305588
#define PD_NOACC                       -305590
#define PD_NOACCACT                    -305592
#define PD_NOACRACC                    -305594
#define PD_NOACRACCACT                 -305596
#define PD_NOING                       -305598
#define PD_NOINGACT                    -305600
#define PD_NOCLH                       -305602
#define PD_NOCLHACT                    -305604
#define PD_NOORG                       -305606
#define PD_NOORGACT                    -305608
#define PD_NOCCCST                     -305610
#define PD_NOCCCSTACT                  -305612
#define PD_NOMAR                       -305614
#define PD_NOMARACT                    -305616
#define PD_NOMSU                       -305618
#define PD_NOMSUACT                    -305620
#define PD_NOINS                       -305622
#define PD_NOINSACT                    -305624
#define PD_NOUPPINS                    -305626
#define PD_NOUPPINSACT                 -305628
#define PD_NOFIXINS                    -305630
#define PD_NOFIXINSACT                 -305632
#define PD_NOINT                       -305634
#define PD_NOINTACT                    -305636
#define PD_NOUPPINT                    -305638
#define PD_NOUPPINTACT                 -305640
#define PD_NOCBC                       -305642
#define PD_NOCBCACT                    -305644
#define PD_NOCBG                       -305646
#define PD_NOCBGACT                    -305648
#define PD_NOCBR                       -305650
#define PD_NOCBRACT                    -305652
#define PD_NOCBT                       -305654
#define PD_NOCBTACT                    -305656
#define PD_NOMNU                       -305658
#define PD_NOMNUACT                    -305660
#define PD_NOATYPE                     -305662
#define PD_NOATYPEACT                  -305664
#define PD_NOCIS                       -305666
#define PD_NOCISACT                    -305668
#define PD_NOCOM                       -305670
#define PD_NOCOMACT                    -305672
#define PD_NONEWCOM                    -305674
#define PD_NONEWCOMACT                 -305676
#define PD_NOBRD                       -305678
#define PD_NOBRDACT                    -305680
#define PD_NOPRICBA                    -305682
#define PD_NOPRICBAACT                 -305684
#define PD_NOSECCBA                    -305686
#define PD_NOSECCBAACT                 -305688
#define PD_NODLSCBA                    -305690
#define PD_NODLSCBAACT                 -305692
#define PD_NOALTCOM                    -305694
#define PD_NOALTCOMACT                 -305696
#define PD_NOINC                       -305698
#define PD_NOINCACT                    -305700
#define PD_NOUPPINC                    -305702
#define PD_NOUPPINCACT                 -305704
#define PD_NOACCTYP                    -305706
#define PD_NOACCTYPACT                 -305708
#define PD_NOEXC                       -305710
#define PD_NOEXCACT                    -305712
#define PD_NOCUR                       -305714
#define PD_NOCURACT                    -305716
#define PD_NOCST                       -305718
#define PD_NOCSTACT                    -305720
#define PD_NOPAYCST                    -305722
#define PD_NOPAYCSTACT                 -305724
#define PD_NOCC1CST                    -305726
#define PD_NOCC1CSTACT                 -305728
#define PD_NOCC2CST                    -305730
#define PD_NOCC2CSTACT                 -305732
#define PD_NOUST                       -305734
#define PD_NOUSTACT                    -305736
#define PD_NOUSR                       -305738
#define PD_NOUSRACT                    -305740
#define PD_NOLAN                       -305742
#define PD_NOLANACT                    -305744
#define PD_NOCOU                       -305746
#define PD_NOCOUACT                    -305748
#define PD_NOBSS                       -305750
#define PD_NOBSSACT                    -305752
#define PD_NOEXP                       -305754
#define PD_NOEXPACT                    -305756
#define PD_NOLST                       -305758
#define PD_NOLSTACT                    -305760
#define PD_NOSPS                       -305762
#define PD_NOSPSACT                    -305764
#define PD_NOVPT                       -305766
#define PD_NOVPTACT                    -305768
#define PD_NOVPD                       -305770
#define PD_NOVPDACT                    -305772
#define PD_NOOBL                       -305774
#define PD_NOOBLACT                    -305776
#define PD_NONTD                       -305778
#define PD_NONTDACT                    -305780
#define PD_NOCCL                       -305782
#define PD_NOCCLACT                    -305784
#define PD_NOCCM                       -305786
#define PD_NOCCMACT                    -305788
#define PD_NOSRU                       -305790
#define PD_NOSRUACT                    -305792
#define PD_NOTRT                       -305794
#define PD_NOTRTACT                    -305796
#define PD_NOLIT                       -305798
#define PD_NOLITACT                    -305800
#define PD_NOSNS                       -305802
#define PD_NOSNSACT                    -305804
#define PD_NOTZO                       -305806
#define PD_NOTZOACT                    -305808
#define PD_NOVMS                       -305810
#define PD_NOVMSACT                    -305812
#define PD_NOVOL                       -305814
#define PD_NOVOLACT                    -305816
#define PD_NOINF                       -305818
#define PD_NOINFACT                    -305820
#define PD_NOADC                       -305822
#define PD_NOADCACT                    -305824
#define PD_NOFTB                       -305826
#define PD_NOFTBACT                    -305828
#define PD_NOFRO                       -305830
#define PD_NOFROACT                    -305832
#define PD_NOQUA                       -305834
#define PD_NOQUAACT                    -305836
#define PD_NOOTHQUA                    -305838
#define PD_NOOTHQUAACT                 -305840
#define PD_NOBAS                       -305842
#define PD_NOBASACT                    -305844
#define PD_NOOTHBAS                    -305846
#define PD_NOOTHBASACT                 -305848
#define PD_NOFLT                       -305850
#define PD_NOFLTACT                    -305852
#define PD_NOAFT                       -305854
#define PD_NOAFTACT                    -305856
#define PD_NOPFT                       -305858
#define PD_NOPFTACT                    -305860
#define PD_NOPEV                       -305862
#define PD_NOPEVACT                    -305864
#define PD_NOPOD                       -305866
#define PD_NOPODACT                    -305868
#define PD_NOPRY                       -305870
#define PD_NOPRYACT                    -305872
#define PD_NOEXT                       -305874
#define PD_NOEXTACT                    -305876
#define PD_NOTAX                       -305878
#define PD_NOTAXACT                    -305880
#define PD_NOBNK                       -305882
#define PD_NOBNKACT                    -305884
#define PD_NOCSS                       -305886
#define PD_NOCSSACT                    -305888
#define PD_NOCSB                       -305890
#define PD_NOCSBACT                    -305892
#define PD_NOTRA                       -305894
#define PD_NOTRAACT                    -305896
#define PD_NOPRD                       -305898
#define PD_NOPRDACT                    -305900
#define PD_NOFRT                       -305902
#define PD_NOFRTACT                    -305904
#define PD_NOWHN                       -305906
#define PD_NOWHNACT                    -305908
#define PD_NOSYM                       -305910
#define PD_NOSYMACT                    -305912
#define PD_NOSBS                       -305914
#define PD_NOSBSACT                    -305916
#define PD_NODLVINT                    -305918
#define PD_NODLVINTACT                 -305920
#define PD_NOWIN                       -305922
#define PD_NOWINACT                    -305924
#define PD_NOPUR                       -305926
#define PD_NOPURACT                    -305928
#define PD_NOBBO                       -305930
#define PD_NOBBOACT                    -305932
#define PD_NOCRS                       -305934
#define PD_NOCRSACT                    -305936
#define PD_NOTRC                       -305938
#define PD_NOTRCACT                    -305940
#define PD_NOTRR                       -305942
#define PD_NOTRRACT                    -305944
#define PD_NOSPR                       -305946
#define PD_NOSPRACT                    -305948
#define PD_NOPSR                       -305950
#define PD_NOPSRACT                    -305952
#define PD_NOBASINS                    -305954
#define PD_NOBASINSACT                 -305956
#define PD_NOOTHINS                    -305958
#define PD_NOOTHINSACT                 -305960
#define PD_NOFATY                      -305962
#define PD_NOFATYACT                   -305964
#define PD_NOWCC                       -305966
#define PD_NOWCCACT                    -305968
#define PD_NOLVMS                      -305970
#define PD_NOLVMSACT                   -305972
#define PD_NOHVMS                      -305974
#define PD_NOHVMSACT                   -305976
#define PD_NORNT                       -305978
#define PD_NOCHGORD                    -305980
#define PD_NOFRMMNU                    -305982
#define PD_NOFRMMNUACT                 -305984
#define PD_NOLFA                       -305986
#define PD_NOLFAACT                    -305988
#define PD_NOLNO                       -305990
#define PD_NOLNOACT                    -305992
#define PD_NONOD                       -305994
#define PD_NONODACT                    -305996
#define PD_NOSRI                       -305998
#define PD_NOSRIACT                    -306000
#define PD_NOTDS                       -306002
#define PD_NOTDSACT                    -306004
#define PD_NOTDP                       -306006
#define PD_NOTDPACT                    -306008
#define PD_INVULTIMO                   -306010
#define PD_INVALWCREATE                -306012
#define PD_INVCALCLTDNOT               -306014
#define PD_INVCALCSTARTNOT             -306016
#define PD_INVCALCSTOPSTART            -306018
#define PD_INVCALCEXPSTOP              -306020
#define PD_EXPMOVBACK                  -306022
#define PD_EXPMOVFORW                  -306024
#define PD_LTDMOVBACK                  -306026
#define PD_LTDMOVFORW                  -306028
#define PD_LTDNOTMOVBACK               -306030
#define PD_SETTLMOVBACK                -306032
#define PD_SETTLMOVFORW                -306034
#define PD_NOTMOVBACK                  -306036
#define PD_NOTMOVFORW                  -306038
#define PD_DLVSTARTMOVBACK             -306040
#define PD_DLVSTARTMOVFORW             -306042
#define PD_DLVSTOPMOVBACK              -306044
#define PD_DLVSTOPMOVFORW              -306046
#define PD_STARTMOVBACK                -306048
#define PD_STARTMOVFORW                -306050
#define PD_STARTNOTMOVBACK             -306052
#define PD_STOPMOVBACK                 -306054
#define PD_STOPMOVFORW                 -306056
#define PD_NOOPN                       -306058
#define PD_NOOPNACT                    -306060
#define PD_NOCOP                       -306062
#define PD_NOCOPACT                    -306064
#define PD_OPENONMAR                   -306066
#define PD_COPNOTEMPTY                 -306068
#define PD_COPPRICEEMPTY               -306070
#define PD_NOPMMCST                    -306072
#define PD_NOPMMCSTACT                 -306074
#define PD_NOPRMEXC                    -306076
#define PD_NOPRMEXCACT                 -306078
#define PD_NOCOG                       -306080
#define PD_NOCOGACT                    -306082
#define PD_NOFML                       -306084
#define PD_NOFMLACT                    -306086
#define PD_NOFOV                       -306088
#define PD_NOFOVACT                    -306090
#define PD_NOOBK                       -306092
#define PD_NOOBKACT                    -306094
#define PD_NOPDP                       -306096
#define PD_NOPDPACT                    -306098
#define PD_NOSYI                       -306100
#define PD_NOSYIACT                    -306102
#define PD_NOTSR                       -306104
#define PD_NOTSRACT                    -306106
#define PD_NOEDG                       -306108
#define PD_NOEDGACT                    -306110
#define PD_NOSPG                       -306112
#define PD_NOSPGACT                    -306114
#define PD_ACCASPAYEMPTY               -306116
#define PD_ACCBDXDEALEMPTY             -306118
#define PD_ACCCHKLEGEMPTY              -306120
#define PD_ACCESSTYPEEMPTY             -306122
#define PD_ACCIDEMPTY                  -306124
#define PD_ACCMAXLEGEMPTY              -306126
#define PD_ACCMINQEMPTY                -306128
#define PD_ACCNOEMPTY                  -306130
#define PD_ACCPUBINFEMPTY              -306132
#define PD_ACCSPRCHKEMPTY              -306134
#define PD_ACREMPTY                    -306136
#define PD_ACRACCIDEMPTY               -306138
#define PD_ACTFROMEMPTY                -306140
#define PD_ACTIVEEMPTY                 -306142
#define PD_ACTPASSEMPTY                -306144
#define PD_ADCIDEMPTY                  -306146
#define PD_AFTIDEMPTY                  -306148
#define PD_ATYIDEMPTY                  -306150
#define PD_ALARMEMPTY                  -306152
#define PD_APPLEVENTEMPTY              -306154
#define PD_BASEDONEMPTY                -306156
#define PD_BBOIDEMPTY                  -306158
#define PD_BESTPREMPTY                 -306160
#define PD_BLOCKEMPTY                  -306162
#define PD_BNKACCEMPTY                 -306164
#define PD_BNKIDEMPTY                  -306166
#define PD_BSSIDEMPTY                  -306168
#define PD_BUYSELLEMPTY                -306170
#define PD_CBAIDEMPTY                  -306172
#define PD_CBCIDEMPTY                  -306174
#define PD_CBGCODEEMPTY                -306176
#define PD_CBGIDEMPTY                  -306178
#define PD_CBRIDEMPTY                  -306180
#define PD_CBTIDEMPTY                  -306182
#define PD_CCLIDEMPTY                  -306184
#define PD_CCLUNITEMPTY                -306186
#define PD_CCMIDEMPTY                  -306188
#define PD_CHGIDEMPTY                  -306190
#define PD_CISIDEMPTY                  -306192
#define PD_CHAIDEMPTY                  -306194
#define PD_CLASSEMPTY                  -306196
#define PD_CLEAREDEMPTY                -306198
#define PD_CLEXPEMPTY                  -306200
#define PD_CLHIDEMPTY                  -306202
#define PD_CLOSALLWEMPTY               -306204
#define PD_CLOSESTSEMPTY               -306206
#define PD_COMIDEMPTY                  -306208
#define PD_COMBILLEMPTY                -306210
#define PD_CONTRSZEMPTY                -306212
#define PD_CONTROPEMPTY                -306214
#define PD_CONTRSHREMPTY               -306216
#define PD_CONTRSIZOPEMPTY             -306218
#define PD_CONVDATEEMPTY               -306220
#define PD_COUIDEMPTY                  -306222
#define PD_COUPONEMPTY                 -306224
#define PD_CROSSPREMPTY                -306226
#define PD_CRSIDEMPTY                  -306228
#define PD_CSBIDEMPTY                  -306230
#define PD_CSSIDEMPTY                  -306232
#define PD_CSTIDEMPTY                  -306234
#define PD_CURIDEMPTY                  -306236
#define PD_CUSTCODEEMPTY               -306238
#define PD_DAILYMTHEMPTY               -306240
#define PD_DATEEMPTY                   -306242
#define PD_DAYSEMPTY                   -306244
#define PD_DEALOPEMPTY                 -306246
#define PD_DEFACCEMPTY                 -306248
#define PD_DEFPOSEMPTY                 -306250
#define PD_DELIVEMPTY                  -306252
#define PD_DELIVINSEMPTY               -306254
#define PD_DELNTDEMPTY                 -306256
#define PD_DERIVEMPTY                  -306258
#define PD_DLSCBAIDEMPTY               -306260
#define PD_DLSDATEEMPTY                -306262
#define PD_DLVINTIDEMPTY               -306264
#define PD_DLVLISTEMPTY                -306266
#define PD_DLVSTARTNTDEMPTY            -306268
#define PD_DLVSTOPNTDEMPTY             -306270
#define PD_EVNOEMPTY                   -306272
#define PD_EXCCODEEMPTY                -306274
#define PD_EXCIDEMPTY                  -306276
#define PD_EXCLEMPTY                   -306278
#define PD_EXPFREQEMPTY                -306280
#define PD_EXPDATEEMPTY                -306282
#define PD_EXPIDEMPTY                  -306284
#define PD_EXPNTDEMPTY                 -306286
#define PD_EXPPERIODEMPTY              -306288
#define PD_FACILITYEMPTY               -306290
#define PD_FACTYPEMPTY                 -306292
#define PD_FASTMARCREMPTY              -306294
#define PD_FASTMARTIMEEMPTY            -306296
#define PD_FIELDCEMPTY                 -306298
#define PD_FILLKILLEMPTY               -306300
#define PD_FIRMEMPTY                   -306302
#define PD_FLTIDEMPTY                  -306304
#define PD_FROMEMPTY                   -306306
#define PD_FROMMENUEMPTY               -306308
#define PD_FTBIDEMPTY                  -306310
#define PD_GENSEREMPTY                 -306312
#define PD_GROUPSETTLEMPTY             -306314
#define PD_INCIDEMPTY                  -306316
#define PD_INDEXEMPTY                  -306318
#define PD_INFALWDEMPTY                -306320
#define PD_INGIDEMPTY                  -306322
#define PD_INSIDEMPTY                  -306324
#define PD_INTBDXDEALEMPTY             -306326
#define PD_INTCHKLEGEMPTY              -306328
#define PD_INTERPEMPTY                 -306330
#define PD_INTERPREMPTY                -306332
#define PD_INTIDEMPTY                  -306334
#define PD_INTMAXLEGEMPTY              -306336
#define PD_INTNOTTRADED                -306338
#define PD_INTSPRCHKEMPTY              -306340
#define PD_IPADDREMPTY                 -306342
#define PD_ISINASSEMPTY                -306344
#define PD_ISAPIEMPTY                  -306346
#define PD_ISCLOSEEMPTY                -306348
#define PD_ISFRACTIONSEMPTY            -306350
#define PD_ISEXCLEMPTY                 -306352
#define PD_ISMMEMPTY                   -306354
#define PD_ISNIDEMPTY                  -306356
#define PD_ISNLOWEMPTY                 -306358
#define PD_ISNHIGHEMPTY                -306360
#define PD_ISNETEMPTY                  -306362
#define PD_ISOIEMPTY                   -306364
#define PD_ISOPENEMPTY                 -306366
#define PD_ISSDATEEMPTY                -306368
#define PD_ISSUEMARKEMPTY              -306370
#define PD_ISTRADEREMPTY               -306372
#define PD_ISTRANSEMPTY                -306374
#define PD_ISTRTRDEVTEMPTY             -306376
#define PD_LANIDEMPTY                  -306378
#define PD_LEGNOEMPTY                  -306380
#define PD_LFAIDEMPTY                  -306382
#define PD_LNOIDEMPTY                  -306384
#define PD_LIFETIMEEMPTY               -306386
#define PD_LITIDEMPTY                  -306388
#define PD_LOWLSTEMPTY                 -306390
#define PD_LSTIDEMPTY                  -306392
#define PD_LTRNTDEMPTY                 -306394
#define PD_MANDPREMPTY                 -306396
#define PD_MANDVOLEMPTY                -306398
#define PD_MARIDEMPTY                  -306400
#define PD_MAXCOMBOLEGEMPTY            -306402
#define PD_MAXTIMEEMPTY                -306404
#define PD_MAXUNITEMPTY                -306406
#define PD_MAXVOLEMPTY                 -306408
#define PD_MAXWAITEMPTY                -306410
#define PD_MEMTYPEMPTY                 -306412
#define PD_MENUEMPTY                   -306414
#define PD_MENUTYPEEMPTY               -306416
#define PD_MENUNOEMPTY                 -306418
#define PD_MINHOLDEMPTY                -306420
#define PD_MINQEMPTY                   -306422
#define PD_MINTIMEEMPTY                -306424
#define PD_MINUNITEMPTY                -306426
#define PD_MINVOLEMPTY                 -306428
#define PD_MINWAITEMPTY                -306430
#define PD_MONTHCODEEMPTY              -306432
#define PD_MONTHNAMEEMPTY              -306434
#define PD_MONTHNOEMPTY                -306436
#define PD_MPORCLEMPTY                 -306438
#define PD_NAMEEMPTY                   -306440
#define PD_NEARESTEMPTY                -306442
#define PD_NEWINSEMPTY                 -306444
#define PD_NOCYCLEMPTY                 -306446
#define PD_NODENAMEEMPTY               -306448
#define PD_NODENUMEMPTY                -306450
#define PD_NOOFDLVEMPTY                -306452
#define PD_NOOFINITEMPTY               -306454
#define PD_NOOFLEGSEMPTY               -306456
#define PD_NOSERIESEMPTY               -306458
#define PD_NOTNTDEMPTY                 -306460
#define PD_NOTPERIODEMPTY              -306462
#define PD_NTDIDEMPTY                  -306464
#define PD_OBLIDEMPTY                  -306466
#define PD_OBLRIGHTSEMPTY              -306468
#define PD_OLDINSEMPTY                 -306470
#define PD_ORDNOEMPTY                  -306472
#define PD_OPENCLOSEEMPTY              -306474
#define PD_OPENSTSEMPTY                -306476
#define PD_OPIFBUYEMPTY                -306478
#define PD_OPIFSELLEMPTY               -306480
#define PD_ORDEREMPTY                  -306482
#define PD_ORGIDEMPTY                  -306484
#define PD_OVRBIDEMPTY                 -306486
#define PD_PASSWORDEMPTY               -306488
#define PD_PAYTYPEEMPTY                -306490
#define PD_PEVIDEMPTY                  -306492
#define PD_PFTIDEMPTY                  -306494
#define PD_PHYSADDREMPTY               -306496
#define PD_PODIDEMPTY                  -306498
#define PD_POSTYPEMPTY                 -306500
#define PD_PRDDATEEMPTY                -306502
#define PD_PRDEVEMPTY                  -306504
#define PD_PRDIDEMPTY                  -306506
#define PD_PRDOMNETEMPTY               -306508
#define PD_PRDVENDOREMPTY              -306510
#define PD_PREMUNITEMPTY               -306512
#define PD_PRICEUNITEMPTY              -306514
#define PD_PRICEQEMPTY                 -306516
#define PD_PRICBAIDEMPTY               -306518
#define PD_PRIDAYSEMPTY                -306520
#define PD_PRIMMARKEMPTY               -306522
#define PD_PRINOTEMPTY                 -306524
#define PD_PRIUNITEMPTY                -306526
#define PD_PROPEMPTY                   -306528
#define PD_PSRIDEMPTY                  -306530
#define PD_PUBIDEMPTY                  -306532
#define PD_PURIDEMPTY                  -306534
#define PD_QREQBDXEMPTY                -306536
#define PD_RACCESSEMPTY                -306538
#define PD_WACCESSEMPTY                -306540
#define PD_RATIOEMPTY                  -306542
#define PD_REALTIMEEMPTY               -306544
#define PD_REFNTDEMPTY                 -306546
#define PD_REJACCEMPTY                 -306548
#define PD_REJTIMOUTEMPTY              -306550
#define PD_RESPEMPTY                   -306552
#define PD_RISKEMPTY                   -306554
#define PD_RNTEMPTY                    -306556
#define PD_SBSIDEMPTY                  -306558
#define PD_SBSSTATUSEMPTY              -306560
#define PD_SECCBAIDEMPTY               -306562
#define PD_SECDAYSEMPTY                -306564
#define PD_SEGMNOEMPTY                 -306566
#define PD_SETTLDAYSEMPTY              -306568
#define PD_SIZEEMPTY                   -306570
#define PD_SHORTNAMEEMPTY              -306572
#define PD_SOSTRKPROPEMPTY             -306574
#define PD_SOSTRKPRFACEMPTY            -306576
#define PD_SOCONTRSZOPEMPTY            -306578
#define PD_SOCONTRSZFACEMPTY           -306580
#define PD_SODEALPROPEMPTY             -306582
#define PD_SODEALPRFACEMPTY            -306584
#define PD_SNSIDEMPTY                  -306586
#define PD_SNSUNDEREMPTY               -306588
#define PD_SNSNOOFDECEMPTY             -306590
#define PD_SNSDECEMPTY                 -306592
#define PD_SNSISSID1EMPTY              -306594
#define PD_SNSISSCODEEMPTY             -306596
#define PD_SNSSIGNEMPTY                -306598
#define PD_SNSSIGNCODEEMPTY            -306600
#define PD_SPREADEMPTY                 -306602
#define PD_SPRIDEMPTY                  -306604
#define PD_SPSIDEMPTY                  -306606
#define PD_SRUIDEMPTY                  -306608
#define PD_SRUONEHIGH                  -306610
#define PD_SRUONELOW                   -306612
#define PD_SRUDTIMENOTEMPTY            -306614
#define PD_STARTPROCEMPTY              -306616
#define PD_STATUSEMPTY                 -306618
#define PD_STEPEMPTY                   -306620
#define PD_STEPSFROMUNDEMPTY           -306622
#define PD_STEPSINCYCEMPTY             -306624
#define PD_STOPTIMEEMPTY               -306626
#define PD_STOPTYPEEMPTY               -306628
#define PD_STIMULIEMPTY                -306630
#define PD_STLNTDEMPTY                 -306632
#define PD_STRIKECODEEMPTY             -306634
#define PD_STREXISTEMPTY               -306636
#define PD_STROPEMPTY                  -306638
#define PD_STRUNITEMPTY                -306640
#define PD_SUBSYSEMPTY                 -306642
#define PD_SYMIDEMPTY                  -306644
#define PD_TAXIDEMPTY                  -306646
#define PD_TIMEUNITEMPTY               -306648
#define PD_TIMEBTWTRADEEMPTY           -306650
#define PD_TOEMPTY                     -306652
#define PD_TRADDATEEMPTY               -306654
#define PD_TRADDAYSEMPTY               -306656
#define PD_TRADEDEMPTY                 -306658
#define PD_TRADTIMEEMPTY               -306660
#define PD_TRADTYPEMPTY                -306662
#define PD_TRCIDEMPTY                  -306664
#define PD_TRDREPRULEEMPTY             -306666
#define PD_TRRIDEMPTY                  -306668
#define PD_TRTIDEMPTY                  -306670
#define PD_TSSIDEMPTY                  -306672
#define PD_TZOIDEMPTY                  -306674
#define PD_TYPEEMPTY                   -306676
#define PD_ULTIMO1EMPTY                -306678
#define PD_ULTIMO2EMPTY                -306680
#define PD_USEMODDUPLEMPTY             -306682
#define PD_CALCLTDNOTEMPTY             -306684
#define PD_CALCSTARTNOTEMPTY           -306686
#define PD_CALCSTOPSTARTEMPTY          -306688
#define PD_CALCEXPSTOPEMPTY            -306690
#define PD_UNDOPEMPTY                  -306692
#define PD_UPPIDEMPTY                  -306694
#define PD_UPPINTIDEMPTY               -306696
#define PD_USRIDEMPTY                  -306698
#define PD_USTIDEMPTY                  -306700
#define PD_VALIDLASTEMPTY              -306702
#define PD_VPDIDEMPTY                  -306704
#define PD_XAXISEMPTY                  -306706
#define PD_YAXISEMPTY                  -306708
#define PD_VOLIDEMPTY                  -306710
#define PD_WCCIDEMPTY                  -306712
#define PD_WHENEMPTY                   -306714
#define PD_WINIDEMPTY                  -306716
#define PD_DESCEMPTY                   -306718
#define PD_MSTCLHIDEMPTY               -306720
#define PD_BASCURIDEMPTY               -306722
#define PD_EXTIDEMPTY                  -306724
#define PD_EXTIDNOTEMPTY               -306726
#define PD_MAXSPANEMPTY                -306728
#define PD_PONESPREMPTY                -306730
#define PD_PSECSPREMPTY                -306732
#define PD_PREGEMPTY                   -306734
#define PD_PALLEMPTY                   -306736
#define PD_HVSTPEMPTY                  -306738
#define PD_WVSTPEMPTY                  -306740
#define PD_FLRTSTPEMPTY                -306742
#define PD_PCDAYEMPTY                  -306744
#define PD_ERTRDEMPTY                  -306746
#define PD_SWTRDEMPTY                  -306748
#define PD_FIXBCEMPTY                  -306750
#define PD_PRIMPBEMPTY                 -306752
#define PD_SECPBEMPTY                  -306754
#define PD_CORREMPTY                   -306756
#define PD_ITRHIGHEMPTY                -306758
#define PD_ITRACREMPTY                 -306760
#define PD_ITRMAXEMPTY                 -306762
#define PD_CALCMEMPTY                  -306764
#define PD_PRINTPEMPTY                 -306766
#define PD_BVTIMEMPTY                  -306768
#define PD_HMAXEMPTY                   -306770
#define PD_NEGTIMEMPTY                 -306772
#define PD_VOLUSDEMPTY                 -306774
#define PD_USRNAMEEMPTY                -306776
#define PD_FASTMARKEMPTY               -306778
#define PD_MAXOMNTRTEMPTY              -306780
#define PD_BRDTYPEEMPTY                -306782
#define PD_DISCUNITEMPTY               -306784
#define PD_DISCOUNTEMPTY               -306786
#define PD_PRCALCMEMPTY                -306788
#define PD_DLVSTARTEMPTY               -306790
#define PD_DLVSTOPEMPTY                -306792
#define PD_STARTDATEEMPTY              -306794
#define PD_STOPDATEEMPTY               -306796
#define PD_AUTOEMPTY                   -306798
#define PD_MSUIDEMPTY                  -306800
#define PD_SUPTRPEMPTY                 -306802
#define PD_SUPBPEMPTY                  -306804
#define PD_SUPLTREMPTY                 -306806
#define PD_SUPITREMPTY                 -306808
#define PD_SUPUNCRSEMPTY               -306810
#define PD_PRINFEMPTY                  -306812
#define PD_VOLINFEMPTY                 -306814
#define PD_DELIVMARGEMPTY              -306816
#define PD_PAYMARGEMPTY                -306818
#define PD_USEDIVIDEMPTY               -306820
#define PD_OPENINTEMPTY                -306822
#define PD_ENABLEOPENEMPTY             -306824
#define PD_BESTPROPENEMPTY             -306826
#define PD_OPNIDEMPTY                  -306828
#define PD_OPENTIMEEMPTY               -306830
#define PD_CLOSETIMEEMPTY              -306832
#define PD_COPLESSEMPTY                -306834
#define PD_COPCHGEMPTY                 -306836
#define PD_COPIDEMPTY                  -306838
#define PD_OPENALLSTOPEMPTY            -306840
#define PD_COGCODEEMPTY                -306842
#define PD_COGIDEMPTY                  -306844
#define PD_OBKCODEEMPTY                -306846
#define PD_OBKIDEMPTY                  -306848
#define PD_TSRIDEMPTY                  -306850
#define PD_EDGIDEMPTY                  -306852
#define PD_SPGIDEMPTY                  -306854
#define PD_FMLIDEMPTY                  -306856
#define PD_FOVIDEMPTY                  -306858
#define PD_PDPIDEMPTY                  -306860
#define PD_SYIIDEMPTY                  -306862
#define PD_PRMEXCIDEMPTY               -306864
#define PD_MINBBOEMPTY                 -306866
#define PD_MKTORDEMPTY                 -306868
#define PD_LMTMKTEMPTY                 -306870
#define PD_OPNRECEMPTY                 -306872
#define PD_MAXFML                      -306874
#define PD_SRIIDEMPTY                  -306876
#define PD_BIDNOTASKEMPTY              -306878
#define PD_BIDANDASKEMPTY              -306880
#define PD_ASKNOTBIDEMPTY              -306882
#define PD_ASKANDBIDEMPTY              -306884
#define PD_SPREADFROMEMPTY             -306886
#define PD_MINBIDPRICEEMPTY            -306888
#define PD_MINASKPRICEEMPTY            -306890
#define PD_PRICEEMPTY                  -306892
#define PD_ABSPEREMPTY                 -306894
#define PD_MINSPREADEMPTY              -306896
#define PD_MAXSPREADEMPTY              -306898
#define PD_ROUNDINGEMPTY               -306900
#define PD_TDPIDEMPTY                  -306902
#define PD_TDSIDEMPTY                  -306904
#define PD_DELAYLESSCOP                -306906
#define PD_DOUBLERISKP                 -306908
#define PD_ISSINFUTURE                 -306910
#define PD_REFTOACC                    -306912
#define PD_REFTOACR                    -306914
#define PD_REFTOCBC                    -306916
#define PD_REFTOCBS                    -306918
#define PD_REFTOCBT                    -306920
#define PD_REFTOCCL                    -306922
#define PD_REFTOCLH                    -306924
#define PD_REFTOCLI                    -306926
#define PD_REFTOCOM                    -306928
#define PD_REFTOCSL                    -306930
#define PD_REFTOCSS                    -306932
#define PD_REFTOCST                    -306934
#define PD_REFTOCUP                    -306936
#define PD_REFTOEXC                    -306938
#define PD_REFTOFVL                    -306940
#define PD_REFTOICO                    -306942
#define PD_REFTOILU                    -306944
#define PD_REFTOINC                    -306946
#define PD_REFTOINS                    -306948
#define PD_REFTOINT                    -306950
#define PD_REFTOISN                    -306952
#define PD_REFTOISS                    -306954
#define PD_REFTOITU                    -306956
#define PD_REFTOLGP                    -306958
#define PD_REFTOLIT                    -306960
#define PD_REFTOLIU                    -306962
#define PD_REFTOLNO                    -306964
#define PD_REFTOLTT                    -306966
#define PD_REFTOLTU                    -306968
#define PD_REFTOMAC                    -306970
#define PD_REFTOMAR                    -306972
#define PD_REFTOMNA                    -306974
#define PD_REFTOORG                    -306976
#define PD_REFTOPAYCST                 -306978
#define PD_REFTOPEV                    -306980
#define PD_REFTOPFT                    -306982
#define PD_REFTOPQR                    -306984
#define PD_REFTOPUR                    -306986
#define PD_REFTOSYM                    -306988
#define PD_REFTOTAX                    -306990
#define PD_REFTOTRC                    -306992
#define PD_REFTOUSR                    -306994
#define PD_REFTOMNU                    -306996
#define PD_REFTOOBL                    -306998
#define PD_REFTOCOG                    -307000
#define PD_REFTOFML                    -307002
#define PD_REFTOFOV                    -307004
#define PD_REFTOOBK                    -307006
#define PD_REFTOPDP                    -307008
#define PD_REFTOSYI                    -307010
#define PD_REFTOTSR                    -307012
#define PD_REFTOSRI                    -307014
#define PD_REFTOTDS                    -307016
#define PD_REFTOTDP                    -307018
#define PD_SYSERR                      -307020
#define PD_INSFMEM                     -307022
#define PD_ERRNOEXP                    -307024
#define PD_ERRNOMAR                    -307026
#define PD_ERRNOINT                    -307028
#define PD_ERRNOINC                    -307030
#define PD_ERRNOCCL                    -307032
#define PD_FLDTYPENOTEX                -307034
#define PD_FLDNOTEX                    -307036
#define PD_WRONGVERSION                -307038
#define PD_VPCNOINIT                   -307040
#define PD_VPCNOANS                    -307042
#define PD_SCODENOTCREATED             -307044
#define PD_MCODENOTCREATED             -307046
#define PD_WRUPDNOTALLOWED             -307048
#define PD_UPDNOTBOTHSYM               -307050
#define PD_OMDUNOTEVT                  -307052
#define PD_ILLEGFAC                    -307054
#define PD_ILLSETTLPROP                -307056
#define PD_SERISTRADED                 -307058
#define PD_ALLOWUNINTEMPTY             -307060
#define PD_WRONGUNDATTR                -307062
#define PD_WRONGEXPATTR                -307064
#define PD_WRONGDEDATTR                -307066
#define PD_QRYONNNOTSUPP               -307068
#define PD_COMBODEALEMPTY              -307070
#define PD_COUNTSIGNEMPTY              -307072
#define PD_NOCLP                       -307074
#define PD_NOCLPACT                    -307076
#define PD_CLPIDEMPTY                  -307078
#define PD_NETTMTHEMPTY                -307080
#define PD_DIFFCLP                     -307082
#define PD_STEPNOTMULT                 -307084
#define PD_LOWZEROMISS                 -307086
#define PD_ISSNOOPRA                   -307088
#define PD_SONOOPRA                    -307090
#define PD_SOISSOPRAISEQ               -307092
#define PD_SOOPRAISEQ                  -307094
#define PD_ISSOPRAISEQ                 -307096
#define PD_ISSOPRAEXIST                -307098
#define PD_SOOPRAEXIST                 -307100
#define PD_NEWINSIDEXIST               -307102
#define PD_NEWINSCODEEXIST             -307104
#define PD_SOINSIDEXIST                -307106
#define PD_SOINSCODEEXIST              -307108
#define PD_CLPCSSIDEMPTY               -307110
#define PD_MMLCKTIME                   -307112
#define PD_BLKRSPTIME                  -307114
#define PD_BLKMINSIZE                  -307116
#define PD_ISSTYPEEMPTY                -307118
#define PD_NOTALLSPG                   -307120
#define PD_TOOMANYSPG                  -307122
#define PD_DOUBLESPG                   -307124
#define PD_DOUBLEEDG                   -307126
#define PD_SYISYMEXIST                 -307128
#define PD_TYPEISTRADED                -307130
#define PD_CBTEXISTS                   -307132
#define PD_MARNOTTRADED                -307134
#define PD_CLASSISTRADED               -307136
#define PD_IMPLNOTINFUT                -307138
#define PD_INTRADAYNOTALLWD            -307140
#define PD_TOTWCNOTALLWD               -307142
#define PD_CLQRYFAIL                   -307144
#define PD_INFOQRYFAIL                 -307146
#define PD_IDCHANGED                    307147
#define PD_INCCHANGED                   307149
#define PD_EXPCHANGED                   307151
#define PD_LTTCHANGED                   307153
#define PD_STRCHANGED                   307155
#define PD_ISNCHANGED                   307157
#define PD_NORELCHANGED                 307159
#define PD_ISSUELSTONLY                -307160
#define PD_ACTIVELSTONLY               -307162
#define PD_LSTCHGNOTALLOWED            -307164
#define PD_BOTHUNDNOTALLOWED           -307166
#define PD_NEWMARNOTALLOWED            -307168
#define PD_DECISNOTEQ                  -307170
#define PD_TSEIDEMPTY                  -307172
#define PD_TIMEEMPTY                   -307174
#define PD_STATEEMPTY                  -307176
#define PD_DOUBLETIME                  -307178
#define PD_NOTSE                       -307180
#define PD_NOTSEACT                    -307182
#define PD_CSTATEWR                    -307184
#define PD_PSTATEWR                    -307186
#define PD_ASTATEWR                    -307188
#define PD_RSTATEWR                    -307190
#define PD_ILLSTARTPROC                -307192
#define PD_STARTINTOVR                 -307194
#define PD_LTTIDEMPTY                  -307196
#define PD_NOLTT                       -307198
#define PD_NOLTTACT                    -307200
#define PD_VALIDFROMEMPTY              -307202
#define PD_VALIDUNTILEMPTY             -307204
#define PD_SOMODEMPTY                  -307206
#define PD_NOINSTEMPLFOUND             -307208
#define PD_NOINCTEMPLFOUND             -307210
#define PD_NOTMCOMFOUND                -307212
#define PD_BOTHNEWSPEMPTY              -307214
#define PD_TSEDIFFNTD                  -307216
#define PD_ONETEMPALLWD                -307218
#define PD_CALCLTDEXPEMPTY             -307220
#define PD_INVLTDCORR                  -307222
#define PD_OPRAINDEXEMPTY              -307224
#define PD_DOUBLEEXC                   -307226
#define PD_ACTCRSEMPTY                 -307228
#define PD_SSTIDEMPTY                  -307230
#define PD_VOLINTEMPTY                 -307232
#define PD_AGGRPAYEMPTY                -307234
#define PD_RESERVSTATE                 -307236
#define PD_HALFDAYEMPTY                -307238
#define PD_NOSST                       -307240
#define PD_NOSSTACT                    -307242
#define PD_CNTRSZISNOTEQ               -307244
#define PD_WREXPOP                     -307246
#define PD_WREXPFAC                    -307248
#define PD_REFTOSBS                    -307250
#define PD_SWBIDEMPTY                  -307252
#define PD_REFINSEMPTY                 -307254
#define PD_LOCKREFEMPTY                -307256
#define PD_DOUBLESWB                   -307258
#define PD_NOREFINS                    -307260
#define PD_NOREFINSACT                 -307262
#define PD_NOSWB                       -307264
#define PD_SWBALRSUSP                  -307266
#define PD_SWBALRRESUMED               -307268
#define PD_NOTLOCKED                   -307270
#define PD_CEGIDEMPTY                  -307272
#define PD_DOUBLECEG                   -307274
#define PD_MATURITYEMPTY               -307276
#define PD_FCOUPONEMPTY                -307278
#define PD_CUPFREQEMPTY                -307280
#define PD_NOOUTRREFINS                -307282
#define PD_NOOUTRREFINSACT             -307284
#define PD_NOOUTRINS                   -307286
#define PD_NOOUTRINSACT                -307288
#define PD_OUTRREFINSEMPTY             -307290
#define PD_OUTRINSEMPTY                -307292
#define PD_IPRIDEMPTY                  -307294
#define PD_NOIPR                       -307296
#define PD_NOIPRACT                    -307298
#define PD_NOREFSPEC                   -307300
#define PD_REFSPEC                     -307302
#define PD_DIFFREFSPEC                 -307304
#define PD_NOTSUSPENDED                -307306
#define PD_REFMUSTBESWAP               -307308
#define PD_REFISSWAP                   -307310
#define PD_OUTRREFISSWAP               -307312
#define PD_LEGMUSTBESWAP               -307314
#define PD_OUTRLEGISSWAP               -307316
#define PD_LEGEQREF                    -307318
#define PD_OLEGEQREF                   -307320
#define PD_OLEGEQOREF                  -307322
#define PD_DOUBLEOSWB                  -307324
#define PD_DOUBLEGNO                   -307326
#define PD_PRYTYPEEMPTY                -307328
#define PD_PRYCONVEMPTY                -307330
#define PD_USECUSIPEMPTY               -307332
#define PD_WRONGCUSIP                  -307334
#define PD_OBLEVNOT1OR5                -307336
#define PD_CUSIPTOLONG                 -307338
#define PD_WRLTTTIME                   -307340
#define PD_REFEXISTASREF               -307342
#define PD_LEGEXISTASREF               -307344
#define PD_REFEXISTASLEG               -307346
#define PD_LEGEXISTASLEG               -307348
#define PD_ERRINFRAC                   -307350
#define PD_REFTOSWB                    -307352
#define PD_MARORDACTWRONG              -307354
#define PD_MARORDACTNOTDEF             -307356
#define PD_COUPAFTMAT                  -307358
#define PD_SERIESEXCEED                -307360
#define PD_LOCKEDSWAP                  -307362
#define PD_SWAPNOTINBOX                -307364
#define PD_DESIDEMPTY                  -307366
#define PD_REFTODES                    -307368
#define PD_DGRIDEMPTY                  -307370
#define PD_REFTODGR                    -307372
#define PD_UPPEREMPTY                  -307374
#define PD_VOLCHGEMPTY                 -307376
#define PD_VDSIDEMPTY                  -307378
#define PD_PMMOBLTYPEEMPTY             -307380
#define PD_TRDIDEMPTY                  -307382
#define PD_INFFEEDIDEMPTY              -307384
#define PD_DEPOSIDEMPTY                -307386
#define PD_HEDGEEXCIDEMPTY             -307388
#define PD_HEDGECSTIDEMPTY             -307390
#define PD_DOUBLEEXTID                 -307392
#define PD_NODES                       -307394
#define PD_NODESACT                    -307396
#define PD_NOVDS                       -307398
#define PD_NOVDSACT                    -307400
#define PD_NODHB                       -307402
#define PD_NODHBACT                    -307404
#define PD_DHBIDEMPTY                  -307406
#define PD_DEPOSTYPEEMPTY              -307408
#define PD_DECISDIFF                   -307410
#define PD_NODGR                       -307412
#define PD_ISINMISSING                  307413
#define PD_ALRSIGNALLED                -307414
#define PD_REFTOTSE                    -307416
#define PD_NOORV                       -307418
#define PD_NOORVACT                    -307420
#define PD_ORVIDEMPTY                  -307422
#define PD_HEDGEALGEMPTY               -307424
#define PD_NOMTH                       -307426
#define PD_NOMTHACT                    -307428
#define PD_MTHIDEMPTY                  -307430
#define PD_NOPDR                       -307432
#define PD_NOPDRACT                    -307434
#define PD_PDRIDEMPTY                  -307436
#define PD_BOTHCOMCIS                  -307438
#define PD_DOUBLECIS                   -307440
#define PD_NOATR                       -307442
#define PD_NOATRACT                    -307444
#define PD_NOLAI                       -307446
#define PD_NOLAIACT                    -307448
#define PD_NOPRL                       -307450
#define PD_NOPRLACT                    -307452
#define PD_NOCIB                       -307454
#define PD_NOCIBACT                    -307456
#define PD_NOTIR                       -307458
#define PD_NOTIRACT                    -307460
#define PD_NOIMS                       -307462
#define PD_NOIMSACT                    -307464
#define PD_NOICS                       -307466
#define PD_NOICSACT                    -307468
#define PD_ATRIDEMPTY                  -307470
#define PD_AATIDEMPTY                  -307472
#define PD_DOUBLEAAT                   -307474
#define PD_NOAAT                       -307476
#define PD_NOAATACT                    -307478
#define PD_PRFORMEMPTY                 -307480
#define PD_STRFORMEMPTY                -307482
#define PD_CABFORMEMPTY                -307484
#define PD_SETTLUNITEMPTY              -307486
#define PD_CIBIDEMPTY                  -307488
#define PD_ICSIDEMPTY                  -307490
#define PD_IMSIDEMPTY                  -307492
#define PD_PRIOEMPTY                   -307494
#define PD_CRRATEEMPTY                 -307496
#define PD_DSRATIOEMPTY                -307498
#define PD_AORBEMPTY                   -307500
#define PD_CHARGEEMPTY                 -307502
#define PD_LAIIDEMPTY                  -307504
#define PD_REFTOLAI                    -307506
#define PD_MORETHANONE                 -307508
#define PD_DOUBLEINST                  -307510
#define PD_TIERIDEMPTY                 -307512
#define PD_EXPNOEMPTY                  -307514
#define PD_DOUBLETIER                  -307516
#define PD_TIEREMPTY                   -307518
#define PD_DOUBLEICS                   -307520
#define PD_DOUBLEIMS                   -307522
#define PD_PRLIDEMPTY                  -307524
#define PD_EXTPROVEMPTY                -307526
#define PD_EXTERNIDEMPTY               -307528
#define PD_LTUNITEMPTY                 -307530
#define PD_INTRATEEMPTY                -307532
#define PD_EXPISSPEC                   -307534
#define PD_EXPNOTSPEC                  -307536
#define PD_DOUBLETDP                   -307538
#define PD_DOUBLETDS                   -307540
#define PD_INTTRDREPRULEEMPTY          -307542
#define PD_NOMTM                       -307544
#define PD_NOMTMACT                    -307546
#define PD_ATTRRULEEMPTY               -307548
#define PD_ONLYONEDEFSAX               -307550
#define PD_WRONGDATEFORMAT             -307552
#define PD_NOSID                       -307554
#define PD_NOSIDACT                    -307556
#define PD_SIDIDEMPTY                  -307558
#define PD_CURRFORMEMPTY               -307560
#define PD_REFTOATY                    -307562
#define PD_REFTOWIN                    -307564
#define PD_EXCEEDFORMAT                -307566
#define PD_EXCEEDPRFORMAT              -307568
#define PD_EXCEEDCABFORMAT             -307570
#define PD_COMBOMTHEMPTY               -307572
#define PD_WRONGSTRFORMAT              -307574
#define PD_CBOMINLEGSEXCEED            -307576
#define PD_RFQPRICEMPTY                -307578
#define PD_CLOSINGTRDPREMPTY           -307580
#define PD_STRIKEEXCEEDED              -307582
#define PD_FIXINSIDEMPTY               -307584
#define PD_FIXINSIDNOTEMPTY            -307586
#define PD_MMRESPTYPEEMPTY             -307588
#define PD_TRDAUTHEMPTY                -307590
#define PD_NOLEGSDEFINED               -307592
#define PD_NORRU                       -307594
#define PD_NORRUACT                    -307596
#define PD_NOGMS                       -307598
#define PD_NOGMSACT                    -307600
#define PD_NOGCT                       -307602
#define PD_NOGCTACT                    -307604
#define PD_GCTIDEMPTY                  -307606
#define PD_MAXYEMPTY                   -307608
#define PD_FBBIDEMPTY                  -307610
#define PD_FUTINSEMPTY                 -307612
#define PD_REFTOFBB                    -307614
#define PD_FUTMUSTBEFB                 -307616
#define PD_LEGMUSTBEFBLEG              -307618
#define PD_LEGEQFUT                    -307620
#define PD_OUTRLEGISWRONG              -307622
#define PD_OLEGEQFB                    -307624
#define PD_SEVCHFOUND                  -307626
#define PD_NOCONVPRICE                 -307628
#define PD_DOUBLEFBB                   -307630
#define PD_DOUBLEOFBB                  -307632
#define PD_NOCHFOUND                   -307634
#define PD_DIFFCTDSPEC                 -307636
#define PD_DIFFCONVSPEC                -307638
#define PD_FBEXISTASFB                 -307640
#define PD_LEGEXISTASFB                -307642
#define PD_FBEXISTASLEG                -307644
#define PD_NOFBB                       -307646
#define PD_NOCONVFACTOR                -307648
#define PD_REFTOGCG                    -307650
#define PD_DOUBLEGCT                   -307652
#define PD_GMSIDEMPTY                  -307654
#define PD_DOUBLENOMTR                 -307656
#define PD_RRUIDEMPTY                  -307658
#define PD_HIDDVOLMETHEMPTY            -307660
#define PD_DOUBLERRU                   -307662
#define PD_MORPEMPTY                   -307664
#define PD_MORPSPEC                    -307666
#define PD_STARTDATESPEC               -307668
#define PD_STOPDATESPEC                -307670
#define PD_REPOGENSPEC                 -307672
#define PD_RRUIDSPEC                   -307674
#define PD_GMSIDSPEC                   -307676
#define PD_UNDERLEXIST                 -307678
#define PD_SERIESEXIST                 -307680
#define PD_CLASSEXIST                  -307682
#define PD_DUPLUNDERL                  -307684
#define PD_DUPLSERIES                  -307686
#define PD_DUPLISIN                    -307688
#define PD_NOTMPLINC                   -307690
#define PD_NOTMPLCOM                   -307692
#define PD_NOTMPLINS                   -307694
#define PD_ISSDATEWRONG                -307696
#define PD_MATURDATEWRONG              -307698
#define PD_DATEDDATEWRONG              -307700
#define PD_MATURDATEEMPTY              -307702
#define PD_DATEDDATEEMPTY              -307704
#define PD_FIRSTCOUPWRONG              -307706
#define PD_LASTCOUPWRONG               -307708
#define PD_FIRSTCOUPEMPTY              -307710
#define PD_COUPFREQEMPTY               -307712
#define PD_GCGROUPEMPTY                -307714
#define PD_FITWRONG                    -307716
#define PD_NOMINALISZERO               -307718
#define PD_REDEMPISZERO                -307720
#define PD_WRONGDCR                    -307722
#define PD_NOCTDPRICE                  -307724
#define PD_CTDPRICESPEC                -307726
#define PD_PSPIDEMPTY                  -307728
#define PD_NOPSP                       -307730
#define PD_NOPSPACT                    -307732
#define PD_DEFCOLLIDEMPTY              -307734
#define PD_NODEF                       -307736
#define PD_NODEFACT                    -307738
#define PD_REFTOCDC                    -307740
#define PD_REFTOPSP                    -307742
#define PD_DOUBLEDEF                   -307744
#define PD_WRONGDEF                    -307746
#define PD_NODEFSPEC                   -307748
#define PD_FUTBNOTINBOX                -307750
#define PD_INSLONGALREXIST             -307752
#define PD_CBSLONGALREXIST             -307754
#define PD_STARTTERMEMPTY              -307756
#define PD_ENDTERMEMPTY                -307758
#define PD_TERMLENEMPTY                -307760
#define PD_TERMLENSPEC                 -307762
#define PD_INSIDEXISTASLONG            -307764
#define PD_INSLONGEXISTASID            -307766
#define PD_TMREPONOTALLOWED            -307768
#define PD_SWBTYPEEMPTY                -307770
#define PD_FUTBOXTYPEEMPTY             -307772
#define PD_REFMUSTBETFORW              -307774
#define PD_CHFOUND                     -307776
#define PD_INVFULLDEPTH                -307778
#define PD_DESEXCEMPTY                 -307780
#define PD_PMMOBLTYPEINV               -307782
#define PD_SERIESSTATUSINV             -307784
#define PD_DEPOSTYPEINV                -307786
#define PD_OPERATIONINV                -307788
#define PD_INVIMPLDATE                 -307790
#define PD_WRONGDGRIMPLDATE            -307792
#define PD_INVHEDGEMEMBER              -307794
#define PD_WKPIDEMPTY                  -307796
#define PD_NOWKP                       -307798
#define PD_NOWKPACT                    -307800
#define PD_REFTOWKP                    -307802
#define PD_INVOBLEVEL                  -307804
#define PD_INVEXTFULLDEPTH             -307806
#define PD_INVINTFULLDEPTH             -307808
#define PD_INVDAYINWEEK                -307810
#define PD_INVWEEKINMONTH              -307812
#define PD_RATIOEXCEEDED               -307814
#define PD_NOMCSDATA                   -307816
#define PD_MCSCHKFAILED                -307818
#define PD_DIFFUNDERL                  -307820
#define PD_ILLRATIO                    -307822
#define PD_NOCBX                       -307824
#define PD_CBXIDEMPTY                  -307826
#define PD_BOXTYPEEMPTY                -307828
#define PD_REFTOCBX                    -307830
#define PD_DOUBLEOLEG                  -307832
#define PD_BOXALRSUSP                  -307834
#define PD_BOXALRRESUMED               -307836
#define PD_LEGMUSTBEOPSWPLEG           -307838
#define PD_NOCBS                       -307840
#define PD_NOCBSACT                    -307842
#define PD_SORTTYPEEMPTY               -307844
#define PD_WRONGPRFORMAT               -307846
#define PD_NEGVALNOTALLOWED            -307848
#define PD_NOCCR                       -307850
#define PD_NOCCRACT                    -307852
#define PD_CCRIDEMPTY                  -307854
#define PD_ACCRINTUDEMPTY              -307856
#define PD_CLEANPRUDEMPTY              -307858
#define PD_DAYCCONVEMPTY               -307860
#define PD_EOMCCONVEMPTY               -307862
#define PD_CALCCEMPTY                  -307864
#define PD_SNSFREETEXTEMPTY            -307866
#define PD_SNSNOOFSUBEMPTY             -307868
#define PD_SNSTERMPARAMEMPTY           -307870
#define PD_LONGSNSIDEMPTY              -307872
#define PD_TERMSNSIDEMPTY              -307874
#define PD_INCLOPTPREMEMPTY            -307876
#define PD_NOPRF                       -307878
#define PD_NOPRFACT                    -307880
#define PD_PRFIDEMPTY                  -307882
#define PD_OPTMIDPREMPTY               -307884
#define PD_GROSSORNETEMPTY             -307886
#define PD_DOUBLEPRF                   -307888
#define PD_TERMIDSPEC                  -307890
#define PD_NOIXV                       -307892
#define PD_NOIXVACT                    -307894
#define PD_IXVIDEMPTY                  -307896
#define PD_DOUBLEIXDATE                -307898
#define PD_NOCRO                       -307900
#define PD_NOCROACT                    -307902
#define PD_CROIDEMPTY                  -307904
#define PD_IXDATEEMPTY                 -307906
#define PD_IXVALEMPTY                  -307908
#define CDB_CGEN_UNDZERO               -310001
#define CDB_CGEN_TOMNYCALL             -310002
#define CDB_CGEN_TOMNYPUT              -310004
#define CDB_CGEN_TOMNYFRWD             -310006
#define CDB_CGEN_COMBCRE                310007
#define CDB_CGEN_COMBEXI                310009
#define CDB_CGEN_NEWUNDER               310011
#define CDB_CGEN_ALTUNDER               310013
#define CDB_CGEN_UNDPRICE               310015
#define CDB_CGEN_NUMCOMB                310017
#define CDB_CGEN_DATEIS                 310019
#define CDB_CGEN_MON                    310021
#define CDB_CGEN_TUE                    310023
#define CDB_CGEN_WED                    310025
#define CDB_CGEN_THU                    310027
#define CDB_CGEN_FRI                    310029
#define CDB_CGEN_SAT                    310031
#define CDB_CGEN_SUN                    310033
#define CDB_SLEC_LOGDEF                 315001
#define CDB_SLEC_NOSTORE                315003
#define CDB_SLEC_STORE                  315005
#define CDB_SLEC_INCNOTFOUND           -315006
#define CDB_SLEC_UPPINSNOTFOUND        -315008
#define CDB_SLEC_INSIDFOUND            -315010
#define CDB_SLEC_ISINFOUND             -315012
#define CDB_SLEC_BINFOUND              -315014
#define CDB_MXLD_ACR_CM_MISS           -320000
#define CDB_MXLD_ACC_INSERT             320001
#define CDB_MXLD_ACC_DELETE             320003
#define CDB_MXLD_COM_DELETE             320005
#define CDB_MXLD_COM_EXISTED            320007
#define CDB_MXLD_COM_INSERT             320009
#define CDB_MXLD_COM_UPDATE             320011
#define CDB_MXLD_INC_DELETE             320013
#define CDB_MXLD_INC_EXISTED            320015
#define CDB_MXLD_INC_INSERT             320017
#define CDB_MXLD_INC_UPDATE             320019
#define CDB_MXLD_INS_DELETE             320021
#define CDB_MXLD_INS_EXISTED            320023
#define CDB_MXLD_INS_INSERT             320025
#define CDB_MXLD_INS_UPDATE             320027
#define CDB_MXLD_SYM_INSERT             320029
#define CDB_MXLD_SYM_DELETE             320031
#define CDB_MXLD_SYM_IGNORED            320033
#define CDB_MXLD_USR_DELETE             320035
#define CDB_MXLD_USR_INSERT             320037
#define CDB_MXLD_USR_UPDATE             320039
#define CDB_MXLD_USR_SUSPEND            320041
#define CDB_MXLD_CST_DELETE             320043
#define CDB_MXLD_CST_INSERT             320045
#define CDB_MXLD_ACRONYMREAD            320047
#define CDB_MXLD_BADGEREAD              320049
#define CDB_MXLD_DELMARK                320051
#define CDB_MXLD_MFIRMREAD              320053
#define CDB_MXLD_OPTSYMREAD             320055
#define CDB_MXLD_OPTSERREAD             320057
#define CDB_MXLD_REGTRADREAD            320059
#define CDB_MXLD_SPECREAD               320061
#define CDB_MXLD_COMPL_OK               320063
#define CDB_SGEN_NON_INT_MODE          -325000
#define CDB_SGEN_ABORT_RTR_WAIT        -325002
#define CDB_SGEN_UNDZERO               -325003
#define CDB_SGEN_TOOMANY_CUR_SER       -325004
#define CDB_SGEN_TOOMANY_CUR_GRP       -325006
#define CDB_SGEN_TOOMANY_SERIES        -325008
#define CDB_SGEN_TOOMANY_STRIKES       -325010
#define CDB_SGEN_ICACHE                -325012
#define CDB_SGEN_IGMT                  -325014
#define CDB_SGEN_IFILL                 -325016
#define CDB_SGEN_IINCR                 -325018
#define CDB_SGEN_IMAX                  -325020
#define CDB_SGEN_SERIES_CREATED         325021
#define CDB_SGEN_SERIES_EXISTED         325023
#define CDB_SGEN_NEWUNDER               325025
#define CDB_SGEN_NEW_INST_CLASS         325027
#define CDB_SGEN_UNDPRICE               325029
#define CDB_SGEN_NUMSERIES              325031
#define CDB_SGEN_DATEIS                 325033
#define CDB_SGEN_NO_NEW_SERIES          325035
#define CDB_SGEN_NOISA                  325037
#define CDB_SGEN_NOISN                  325039
#define CDB_SGEN_IWRONG                 325041
#define CDB_SGEN_IGET                   325043
#define CDB_VPCGWY_OMLOGINGIVEUP       -330000
#define CDB_VPCGWY_MQLOGINGIVEUP       -330002
#define CDB_VPCGWY_NOSYSRES            -330004
#define CDB_VPCGWY_RECOVFILEERR        -330006
#define CDB_VPCGWY_MQDEADMSG           -330008
#define CDB_VPCGWY_ADDFAIL             -330010
#define CDB_VPCGWY_OMQRYFAIL           -330012
#define CDB_VPCGWY_INTERR              -330014
#define CDB_VPCGWY_COMERR              -330016
#define CDB_VPCGWY_OMTRANSERR          -330018
#define CDB_VPCGWY_OMLOGINFAIL         -330019
#define CDB_VPCGWY_OMLOGOUTFAIL        -330021
#define CDB_VPCGWY_MQCONNFAIL          -330023
#define CDB_VPCGWY_MQOPENFAIL          -330025
#define CDB_VPCGWY_MQCLOSEFAIL         -330027
#define CDB_VPCGWY_MQDISCFAIL          -330029
#define CDB_VPCGWY_MQPUTFAIL           -330031
#define CDB_VPCGWY_MQGETFAIL           -330033
#define CDB_VPCGWY_MQTRANSMSG          -330035
#define CDB_VPCGWY_OMLOGINSUC           330037
#define CDB_VPCGWY_OMLOGOUTSUC          330039
#define CDB_VPCGWY_MQCONNSUC            330041
#define CDB_VPCGWY_MQOPENSUC            330043
#define CDB_VPCGWY_MQCLOSESUC           330045
#define CDB_VPCGWY_MQDISCSUC            330047
#define CDB_VPCGWY_MQDQEMPTY            330049
#define CDB_VPCGWY_MQTRANSQEMPTY        330051
#define CDB_VPCGWY_ACTSERIES            330053
#define CDB_VPCGWY_VPCINERR             330055
#define CDB_VPCGWY_OMBRREC              330057
#define CDB_VPCGWY_EXIT                 330059
#define CDB_VPCGWY_START                330061

/******************************************************************************
*end*of* cdb_messages.h
*******************************************************************************/

#endif /* _CDB_MESSAGES_H_ */
#ifndef _CL_MESSAGES_H_
#define _CL_MESSAGES_H_

/******************************************************************************
Module: cl_messages.h

Message definitions generated 2001-10-23 02:32:58 by amt, version V2.4.

NOTE: Do not edit this file. It is automatically generated from
      one or more message files.
*******************************************************************************/

#define CL_OK                           700001
#define CL_CMD_QUEUE                    700003
#define CL_COMPLETED                    700005
#define CL_NOQTY                        700007
#define CL_NYI                         -700009
#define CL_INVACCOUNT                  -700011
#define CL_STATEMISMAT                 -700013
#define CL_STATENOTEND                 -700015
#define CL_STATEEND                    -700017
#define CL_PSSTOPUSR                   -700019
#define CL_PSSTOPRSTRT                 -700021
#define CL_PSALRSTOP                   -700023
#define CL_BUFOVFLW                    -700025
#define CL_REQHOLDING                  -700027
#define CL_INVID                       -700028
#define CL_INVMEMBER                   -700030
#define CL_ERRORTBS                    -700032
#define CL_DB_LOCKED                   -700034
#define CL_ILLVERNOUPD                 -700036
#define CL_INVLOGICAL                  -700038
#define CL_NOSERORVAL                  -700040
#define CL_NODEAL                      -700042
#define CL_INVACCORQTY                 -700044
#define CL_INVDEALSOURCE               -700046
#define CL_NODEALPRICE                 -700048
#define CL_INVPOSREQ                   -700050
#define CL_INCBUYSELL                  -700052
#define CL_INVTIME                     -700054
#define CL_INVFIXING                   -700056
#define CL_PRODEVTEXIST                -700058
#define CL_PRODEVTACTIV                -700060
#define CL_FTBNOTFND                   -700062
#define CL_NOFTBORFUNC                 -700064
#define CL_INVOPER                     -700066
#define CL_NOQTYSUPPL                  -700068
#define CL_TRDNOTFND                   -700070
#define CL_TOOMUCHQTY                  -700072
#define CL_NOPOSFND                    -700074
#define CL_TRDNOTSPEC                  -700076
#define CL_TRDSPEC                     -700078
#define CL_EXENOTSUP                   -700080
#define CL_INVEXEREQ                   -700082
#define CL_INVTRANSTYPE                -700084
#define CL_INVTRANSSIZE                -700086
#define CL_PROTOCOLERRE                -700088
#define CL_PROTOCOLERRD                -700090
#define CL_NOITEMS                     -700092
#define CL_TXNOTHANDLED                -700094
#define CL_INTXNDENIED                 -700096
#define CL_INVTRANSRULE                -700098
#define CL_INVTRADESTATE               -700100
#define CL_INCORRECTSUM                -700102
#define CL_INVACCESSW                  -700104
#define CL_INVACCESSR                  -700106
#define CL_INVACCESSE                  -700108
#define CL_POSNOTSUFCLO                -700110
#define CL_INVSTAFORREV                -700112
#define CL_REMNOTQTY                   -700114
#define CL_INVTRDNUM                   -700116
#define CL_TRDNOTTRANS                 -700118
#define CL_INVNBRTRD                   -700120
#define CL_REMTOBIG                    -700122
#define CL_INVTRAIL                    -700124
#define CL_TRANSNOTNEW                 -700126
#define CL_TRANSNOTPLAIN               -700128
#define CL_ADJACCPOS                   -700130
#define CL_INVSERIES                   -700132
#define CL_INVSERORVAL                 -700134
#define CL_INVBASE                     -700136
#define CL_INVULSERIES                 -700138
#define CL_ERRCREACC                   -700140
#define CL_INVACCSTATE                 -700142
#define CL_NOACCCHANGE                 -700144
#define CL_INVACCOUNTE                 -700146
#define CL_INVACCTYP                   -700148
#define CL_INVACCFEETYP                -700150
#define CL_INVCUSTBANK                 -700152
#define CL_INVCOUNTSIGN                -700154
#define CL_ACCISDEFAULT                -700156
#define CL_TRANSANDSINK                -700158
#define CL_INVCHARINACC                -700160
#define CL_ACCOUNT_STATE               -700162
#define CL_ACCOUNT_DUPL                -700164
#define CL_PRODNOTFND                  -700166
#define CL_INVCHGTYP                   -700168
#define CL_INVPSSTATUS                 -700170
#define CL_TMPPRODSTAT                 -700172
#define CL_PRODSTATNOGOOD              -700174
#define CL_LASTSTATE                   -700176
#define CL_FUNCNOTFND                  -700178
#define CL_FUNCTABOVF                  -700180
#define CL_FUNCONLYTRD                 -700182
#define CL_FUNCONLYPOS                 -700184
#define CL_DUPDELITEM                  -700186
#define CL_DELITEMNFND                 -700188
#define CL_PRDDATENFND                 -700190
#define CL_INVWHEN                     -700192
#define CL_FIM_ERROR                   -700194
#define CL_NOPEVRULES                  -700196
#define CL_INVPEVCLASS                 -700198
#define CL_INVXAXISREP                 -700200
#define CL_DECIMLOST                   -700202
#define CL_INVNBRDEC                   -700204
#define CL_PRODNOTSERIES               -700206
#define CL_INVCONTRVAL                 -700208
#define CL_INVCONTRSIZ                 -700210
#define CL_INVSTRIKEPRICE              -700212
#define CL_INVEVTTYPE                  -700214
#define CL_INCPEVDATA                  -700216
#define CL_UNBALDVP                    -700218
#define CL_PANIC                       -700220
#define CL_INVPROGNAME                 -700222
#define CL_INVPROGPARAM                -700224
#define CL_NEWFAILED                   -700226
#define CL_INVLESTATE                  -700228
#define CL_BADEXEQTY                   -700230
#define CL_BADASSIGN                   -700232
#define CL_BADEXEREQ                   -700234
#define CL_NOTRDDEL                    -700236
#define CL_ONLYBINDEALS                -700238
#define CL_TRANSEXIST                  -700240
#define CL_BWNOMATCH                   -700242
#define CL_INVNBRSHARES                -700244
#define CL_INVCOMMODITY                -700246
#define CL_NOSUCHFILE                  -700248
#define CL_ERROPNFIL                   -700250
#define CL_ERRCREFIL                   -700252
#define CL_ERRCLOFIL                   -700254
#define CL_ERRFLUSHFIL                 -700256
#define CL_ERRWRITEFIL                 -700258
#define CL_ERREADFIL                   -700260
#define CL_ENDOFILE                    -700261
#define CL_BUFTOOSMALL                 -700262
#define CL_BADFILFMT                   -700264
#define CL_BADLNKID                    -700266
#define CL_INVALIDLNK                  -700268
#define CL_BADSEQNBR                   -700270
#define CL_DUPLINCACHE                 -700272
#define CL_UNKREPORT                   -700274
#define CL_INVPEVAUTO                  -700276
#define CL_NOIP5DEAL                   -700278
#define CL_INVQTYPARAM                 -700280
#define CL_RTR_ERROR                   -700282
#define CL_INVMASTERPOSCHG             -700284
#define CL_RECOVFAIL                   -700286
#define CL_GDNNOTNEXT                  -700288
#define CL_PRODEVTEXISTSER             -700290
#define CL_NOISSCONVMOD                -700291
#define CL_SRVREJECT                   -700293
#define CL_NOSRVAVAIL                  -700295
#define CL_SERNOTFND                   -700296
#define CL_INVNOMVAL                   -700298
#define CL_INVEXEREQACCTYPE            -700300
#define CL_INVEXEREQACCPROP            -700302
#define CL_UNEXPTRDUPD                 -700304
#define CL_INVAPTQTY                   -700306
#define CL_RRNOTHOLDING                -700308
#define CL_INVEXEREQSTATE              -700310
#define CL_INVEXEREQTYPE               -700312
#define CL_INVPRDEVTREQSTATE           -700314
#define CL_INVEXEREQDATE               -700316
#define CL_INVPRDEVTREQDATE            -700318
#define CL_INVROLLSTATE                -700320
#define CL_INVROLLBACKTYPE             -700322
#define CL_INVROLLBACKPARAMS           -700324
#define CL_SAMEUSER                    -700326
#define CL_INVCANC                     -700328
#define CL_INVREFPRICE                 -700330
#define CL_INVOUTRVAL                  -700332
#define CL_INVSSTNUM                   -700334
#define CL_INVCDCUPD                   -700336
#define CL_DEALNOTSTORED               -700338
#define CL_ROLLNOTENABLED              -700340
#define CL_PROTOCOLERRT                -700342
#define CL_NOTRADEFORAB                -700344
#define CL_NODEALFORAB                 -700346
#define CL_INCONSISTENCYINGIVEUP       -700348
#define CL_KEYTABOFL                   -700350
#define CL_TRDCLDRCHNG                 -700352
#define CL_INVSUBSEVNT                 -700354
#define CL_INVINSTRTYP                 -700356
#define CL_NODUALPOS                   -700358
#define CL_ILLTRNSTIMEINTERVAL         -700360
#define CL_NOTRNSTIMEOBJ               -700362
#define CL_SQLERROR                    -700364
#define CL_OMN_FACILITY_BEGIN           704001
#define CL_OMN_NORMAL                   704003
#define CL_OMN_REQHOLDING               704005
#define CL_OMN_BATNTRN                  704007
#define CL_OMN_NOINFO                   704009
#define CL_OMN_BADARGS                 -704010
#define CL_OMN_UKNWNQRY                -704012
#define CL_OMN_SNDFAIL                 -704014
#define CL_OMN_ALRMOV                  -704016
#define CL_OMN_INVBALISS               -704018
#define CL_OMN_INVBALOWN               -704020
#define CL_OMN_INVCOUNTRY              -704022
#define CL_OMN_INVISSOWN               -704024
#define CL_OMN_INVTRANSNUM             -704026
#define CL_OMN_INVVERNO                -704028
#define CL_OMN_RTRERROR                -704030
#define CL_OMN_TXALRSTORED             -704032
#define CL_OMN_BADPOSUPD               -704034
#define CL_OMN_BADSIZE                 -704036
#define CL_OMN_BADSEG                  -704038
#define CL_OMN_BADSEQ                  -704040
#define CL_OMN_ALRCANC                 -704042
#define CL_OMN_DACEMPTY                -704044
#define CL_OMN_INVACCOUNT              -704046
#define CL_OMN_INVBIDASK               -704048
#define CL_OMN_INVDACACC               -704050
#define CL_OMN_INVDEALSRC              -704052
#define CL_OMN_INVINSTRTYP             -704054
#define CL_OMN_INVITMCNT               -704056
#define CL_OMN_INVMARKET               -704058
#define CL_OMN_INVPRICE                -704060
#define CL_OMN_INVQUANT                -704062
#define CL_OMN_INVSERIES               -704064
#define CL_OMN_INVTRANSTYPE            -704066
#define CL_OMN_NODEALFND               -704068
#define CL_OMN_INVTRDID                -704070
#define CL_OMN_INVMEMBER               -704072
#define CL_OMN_INVCHGREQ               -704074
#define CL_OMN_INVACCESSW              -704076
#define CL_OMN_VALIDERR                -704078
#define CL_OMN_STORERR                 -704080
#define CL_OMN_PRODEVTEXIST            -704082
#define CL_OMN_PRODSTATNOGOOD          -704084
#define CL_OMN_INVACCORQTY             -704086
#define CL_OMN_INVACCESSR              -704088
#define CL_OMN_INVACCESSE              -704090
#define CL_OMN_NOQTYSUPPL              -704092
#define CL_OMN_TRDNOTFND               -704094
#define CL_OMN_TOOMUCHQTY              -704096
#define CL_OMN_NOPOSFND                -704098
#define CL_OMN_TRDNOTSPEC              -704100
#define CL_OMN_TRDSPEC                 -704102
#define CL_OMN_EXENOTSUP               -704104
#define CL_OMN_INVEXEREQ               -704106
#define CL_OMN_INVACCOUNTE             -704108
#define CL_OMN_INVACCSTATE             -704110
#define CL_OMN_INVTRADESTATE           -704112
#define CL_OMN_REMTOBIG                -704114
#define CL_OMN_INVID                   -704116
#define CL_OMN_POSNOTSUFCLO            -704118
#define CL_OMN_INCORRECTSUM            -704120
#define CL_OMN_BADXBALVER              -704122
#define CL_OMN_NOXBALVER               -704124
#define CL_OMN_SIGALRREC               -704126
#define CL_OMN_ILLTRNSTIME             -704128
#define CL_OMN_DB_LOCKED               -704130
#define CL_OMN_INVTIME                 -704132
#define CL_OMN_TXNOTHANDLED            -704134
#define CL_OMN_INTXNDENIED             -704136
#define CL_OMN_INVOPER                 -704138
#define CL_OMN_INTERR                  -704140
#define CL_OMN_INVEXCONF               -704142
#define CL_OMN_INVEXREJECT             -704144
#define CL_OMN_INVTRDCONF              -704146
#define CL_OMN_UNKREPORT               -704148
#define CL_OMN_INVMASTERPOSCHG         -704150
#define CL_OMN_INCBUYSELL              -704152
#define CL_OMN_REMNOTQTY               -704154
#define CL_OMN_INVXACCOP               -704156
#define CL_OMN_ABHOLDTRADE             -704158
#define CL_OMN_INVCMTA                 -704159
#define CL_OMN_INVACCTYPE              -704161
#define CL_OMN_INVOPENCLOSE            -704163
#define CL_OMN_CTRSALRPEND             -704164
#define CL_OMN_CRECTRSALRPEND          -704166
#define CL_OMN_DELCTRSALRPEND          -704168
#define CL_OMN_MODCTRSALRPEND          -704170
#define CL_OMN_CTRSFAILED              -704172
#define CL_OMN_NOPENDCTRS              -704173
#define CL_OMN_INVTRADCODE             -704174
#define CL_OMN_ABPENDAFACTRS           -704176
#define CL_OMN_NODEALSTATE             -704178
#define CL_OMN_BADDEALSTATECHANGE      -704180
#define CL_OMN_TMSERNOTDEF             -704182
#define CL_OMN_INVRCTINTRMTRD          -704184
#define CL_OMN_INVRCTINTRMDEAL         -704186
#define CL_OMN_INVRCTAPTDEAL           -704188
#define CL_OMN_NOTANAPTDEAL            -704190
#define CL_OMN_APTRDNOTFND             -704192
#define CL_OMN_INVTRDTYPRCT            -704194
#define CL_OMN_INVACCEDITRA            -704195
#define CL_OMN_INVACCEDITRAN           -704197
#define CL_OMN_INVACCEDIDATD           -704199
#define CL_OMN_INVACCEDIDATE           -704201
#define CL_OMN_TRANSANDSINK            -704202
#define CL_OMN_TRANSNOAUTONET          -704204
#define CL_OMN_NOACCHNGPROP            -704206
#define CL_OMN_NOMODACCDEL             -704208
#define CL_OMN_NOACCHANGEDEL           -704210
#define CL_OMN_INVBNDQUO               -704212
#define CL_OMN_INVPDBNDQUO             -704214
#define CL_OMN_NOPDMATCH               -704216
#define CL_OMN_PHYSNOTREG              -704217
#define CL_OMN_INVDELIVQTY             -704218
#define CL_OMN_INVCSDACC               -704220
#define CL_OMN_TOOFEWITEMS             -704222
#define CL_OMN_BADDCSEQNBR             -704224
#define CL_OMN_INVPOSREQ               -704226
#define CL_OMN_RECDELIV                -704228
#define CL_OMN_TOOMUCHREG              -704230
#define CL_OMN_PDINHOLD                -704232
#define CL_OMN_DELDISTDONE             -704234
#define CL_OMN_INVSTAFORREV            -704236
#define CL_OMN_TRANSNOTNEW             -704238
#define CL_OMN_MEMSUSPCL               -704240
#define CL_OMN_INVTRANSRULE            -704242
#define CL_OMN_INVOUTRVAL              -704244
#define CL_OMN_TOOMANYLEGS             -704246
#define CL_OMN_TRADEMUSTBEACTIVE       -704248
#define CL_OMN_TRADETYPEINVALID        -704250
#define CL_OMN_INVGIVEUPACCESS         -704252
#define CL_OMN_NOTENOUGHREMAINING      -704254
#define CL_OMN_INVGIVEUPACC            -704256
#define CL_OMN_INVGIVEUPACCSTATE       -704258
#define CL_OMN_INVGIVEUPACCTYPE        -704260
#define CL_OMN_GIVEUPNOTHOLDING        -704262
#define CL_OMN_NOTCORRECTSUM           -704264
#define CL_OMN_NOREJECTACCESS          -704266
#define CL_OMN_NOCONFIRMACCESS         -704268
#define CL_OMN_HOLDINGGIVEUP           -704270
#define CL_OMN_HOLDINGRCTTRD           -704272
#define CL_OMN_MARKSTILOP              -704274
#define CL_OMN_INCONSDEALSRC           -704276
#define CL_OMN_INCONSINSTIG            -704278
#define CL_OMN_INCONSTRADETYPE         -704280
#define CL_OMN_INCTRDTYPORDEASRC       -704282
#define CL_OMN_INCONSBUYSELL           -704284
#define CL_OMN_CLSEQNBRTOOLOW          -704286
#define CL_OMN_CLSEQNBRTOOHIGH         -704288
#define CL_OMN_INVTRDDATA              -704290
#define CL_OMN_INVEXTTRDNUM            -704292
#define CL_OMN_GIVEUPNOINST            -704294
#define CL_OMN_RECTNOREV               -704296
#define CL_OMN_ONLYONEDEAL             -704298
#define CL_OMN_INVCLDATE               -704300
#define CL_OMN_INVACCID                -704302
#define CL_OMN_INVCABIND               -704304
#define CL_OMN_INVCLICAT               -704306
#define CL_OMN_INVEXTTRDFEETYP         -704308
#define CL_OMN_INVEXTDEALNUM           -704310
#define CL_OMN_INVQTY                  -704312
#define CL_OMN_TRANSEXIST              -704314
#define CL_OMN_ACCINVALID              -704316
#define CL_OMN_ACCSTATENCHANGE         -704318
#define CL_OMN_RECURSIVEORIG           -704320
#define CL_OMN_INVORIEXTTRDNUM         -704322
#define CL_OMN_INVACCLVL               -704324
#define CL_OMN_INVSUMFLG               -704326
#define CL_OMN_TRANSNOTPLAIN           -704328
#define CL_OMN_EXTRDNBRNOTUNIQUE       -704330
#define CL_OMN_INVWILDSEARCH           -704332
#define CL_OMN_NODEALPRICE             -704334
#define CL_OMN_UNBALQTY                -704336
#define CL_OMN_INVRESIDUAL             -704338
#define CL_OMN_INVINSTIGANT            -704340
#define CL_OMN_INVTRADETYPE            -704342
#define CL_OMN_NOTALLACCTYPE           -704344
#define CL_OMN_INVFINALFLG             -704346
#define CL_OMN_INVFIXLABEL             -704348
#define CL_OMN_IDMTMNOTCOMPLETE        -704350
#define CL_OMN_TRADETOOOLD             -704352
#define CL_OMN_CLRDATECLOSED           -704354
#define CL_OMN_CLRDATENOTCLOSED        -704356
#define CL_OMN_CANCNOTALLNONBIN        -704358
#define CL_OMN_CANCNOTALLINST          -704360
#define CL_OMN_CANCNOTALLWUINST        -704362
#define CL_OMN_CANCNOTALLBOX           -704364
#define CL_OMN_CANCTOLATE              -704366
#define CL_OMN_CANCNOTALLNONBIL        -704368
#define CL_OMN_RCTNOTALL               -704370
#define CL_OMN_NOANNULDEAL             -704372
#define CL_OMN_CANTNETGUPEXISTS        -704373
#define CL_OMN_FACILITY_END             704375
#define CL_OCC_CHGCLOSETOOPENBUY       -708001
#define CL_OCC_CHGCLOSETOOPENSELL      -708003
#define CL_OCC_CHGCLOSETOOPENBOTH      -708005
#define CL_OCC_NONNUMTRADEDATE         -708007
#define CL_OCC_TRADEDATENOTCUR         -708009
#define CL_OCC_NONNUMTRDEXTIME         -708011
#define CL_OCC_NONNUMTRDNUMBER         -708013
#define CL_OCC_NONNUMTRDPRICE          -708015
#define CL_OCC_NONNUMTRDVOL            -708017
#define CL_OCC_INVCHGCODE              -708019
#define CL_OCC_INVBUYMEM               -708021
#define CL_OCC_INVBUYACC               -708023
#define CL_OCC_INVBUYOPCL              -708025
#define CL_OCC_INVSELLMEM              -708027
#define CL_OCC_INVSELLACC              -708029
#define CL_OCC_INVSELLOPCL             -708031
#define CL_OCC_INVCHGSELLOPCL          -708033
#define CL_OCC_INVCHGBUYOPCL           -708035
#define CL_OCC_INVSERIES               -708037
#define CL_OCC_CLTRDBUYACC             -708039
#define CL_OCC_CLTRDSELLAC             -708041
#define CL_OCC_SELLINSUFPOS            -708043
#define CL_OCC_BUYINSUFPOS             -708045
#define CL_OCC_INVMEMBER               -708047
#define CL_OCC_INVACCOUNT              -708049
#define CL_OCC_EXERCVOLZERO            -708051
#define CL_OCC_INVEXERCVOL             -708053
#define CL_OCC_UNCLTRDCHG              -708055
#define CL_OCC_ALRTRDCHG               -708057
#define CL_OCC_PREMEXBYEX              -708059
#define CL_OCC_DUPLTRDNUMBER           -708061
#define CL_OCC_BUYGUPFORHOUSE          -708063
#define CL_OCC_SELLGUPFORHOUSE         -708065
#define CL_OCC_BUYGUPCHGREQ            -708067
#define CL_OCC_BUYGUPDELREQ            -708069
#define CL_OCC_SELLGUPCHGREQ           -708071
#define CL_OCC_SELLGUPDELREQ           -708073
#define CL_OCC_INVBUYGUP               -708075
#define CL_OCC_INVSELLGUP              -708077
#define CL_OCC_INVBUYSELLGUP           -708079
#define CL_OCC_INVBUYGUPBUYCHG         -708081
#define CL_OCC_INVBUYGUPSELLCHG        -708083
#define CL_OCC_INVBUYGUPBUYSELLCHG     -708085
#define CL_OCC_INVSELLGUPBUYCHG        -708087
#define CL_OCC_INVSELLGUPSELLCHG       -708089
#define CL_OCC_INVSELLGUPBUYSELLCHG    -708091
#define CL_OCC_INVBUYSELLGUPBUYCHG     -708093
#define CL_OCC_INVBUYSELLGUPSELLCHG    -708095
#define CL_OCC_INVBUYSELLGUPBUYSELLCHG -708097
#define CL_DC_TRDCHANGED               -709001
#define CL_DC_TRDREJECT                -709003

/******************************************************************************
*end*of* cl_messages.h
*******************************************************************************/

#endif /* _CL_MESSAGES_H_ */
#ifndef _CX_MESSAGES_H_
#define _CX_MESSAGES_H_

/******************************************************************************
Module: cx_messages.h

Message definitions generated 2001-08-30 16:47:30 by amt, version V2.4.

NOTE: Do not edit this file. It is automatically generated from
      one or more message files.
*******************************************************************************/

#define CX_NORMAL                       1700001
#define CX_TXALRDHDL                    1700003
#define CX_UNKNOWNEVENT                -1700004
#define CX_INVTRANSSIZE                -1700006
#define CX_INVTRANSTYPE                -1700008
#define CX_INTERNALERROR               -1700009
#define CX_INVUSER                     -1700011
#define CX_INVTX                       -1700013
#define CX_INVSEGMENTNR                -1700015
#define CX_INSFMEM                     -1700016
#define CX_BUGCHECK                    -1700018
#define CX_ABORT                       -1700020
#define CX_SQLERROR                    -1700022
#define CX_CARRYMISSING                -1700024
#define CX_TOMANYCARRYLEG              -1700026
#define CX_LABELNOTFOUND               -1700028

/******************************************************************************
*end*of* cx_messages.h
*******************************************************************************/

#endif /* _CX_MESSAGES_H_ */
#ifndef _GC_MESSAGES_H_
#define _GC_MESSAGES_H_

/******************************************************************************
Module: gc_messages.h

Message definitions generated 2001-05-22 11:02:28 by amt, version V2.4.

NOTE: Do not edit this file. It is automatically generated from
      one or more message files.
*******************************************************************************/

#define GC_OK                           7700001
#define GC_CMD_QUEUE                    7700003
#define GC_COMPLETED                    7700005
#define GC_NOQTY                        7700007
#define GC_INVACCOUNT                  -7700009
#define GC_STATEMISMAT                 -7700011
#define GC_BUFOVFLW                    -7700013
#define GC_INVTRADCODE                 -7700014
#define GC_REQHOLDING                  -7700015
#define GC_INVID                       -7700016
#define GC_INVMEMBER                   -7700018
#define GC_ERRORTBS                    -7700020
#define GC_ILLTRNSTIME                 -7700022
#define GC_INVMARKET                   -7700024
#define GC_DB_LOCKED                   -7700026
#define GC_INVLOGICAL                  -7700028
#define GC_INVACCORQTY                 -7700030
#define GC_INVDEALSOURCE               -7700032
#define GC_INCBUYSELL                  -7700034
#define GC_INVTIME                     -7700036
#define GC_PRODEVTEXIST                -7700038
#define GC_NOQTYSUPPL                  -7700040
#define GC_INVTRANSTYPE                -7700042
#define GC_INVTRANSSIZE                -7700044
#define GC_PROTOCOLERRE                -7700046
#define GC_NOITEMS                     -7700048
#define GC_TXNOTHANDLED                -7700050
#define GC_INTXNDENIED                 -7700052
#define GC_INVACCESSW                  -7700054
#define GC_INVACCESSR                  -7700056
#define GC_INVSERIES                   -7700058
#define GC_INVNBRDEC                   -7700060
#define GC_PANIC                       -7700062
#define GC_INVPROGNAME                 -7700064
#define GC_INVPROGPARAM                -7700066
#define GC_ONLYBINDEALS                -7700068
#define GC_BADSEQNBR                   -7700070
#define GC_RTR_ERROR                   -7700072
#define GC_GDNNOTNEXT                  -7700074
#define GC_SRVREJECT                   -7700075
#define GC_NOSRVAVAIL                  -7700077
#define GC_INVREFPRICE                 -7700078
#define GC_INVDEALNUMBER               -7700080
#define GC_INVTRADENUMBER              -7700082
#define GC_INVALLOCATION_ITEMNUMBER    -7700084
#define GC_INVALLOCATIONNUMBER         -7700086
#define GC_INVMEMBERBUY                -7700088
#define GC_INVMEMBERSELL               -7700090
#define GC_INVQUANTITY                 -7700092
#define GC_INVDEALPRICE                -7700094
#define GC_INVCHECKDISABLED            -7700095
#define GC_INVMAXSEC                   -7700096
#define GC_INVDEAL                     -7700098
#define GC_NOTFOUND                    -7700100
#define GC_DUPLICATE                   -7700102
#define GC_INVTIMELIMITVALUE           -7700104
#define GC_NOMRKTPRICE                 -7700105
#define GC_QTYCHANGED                  -7700106
#define GC_VALUECHANGED                -7700108
#define GC_COLLVALUECHANGED            -7700110
#define GC_ORIGITMNBRCHANGED           -7700112
#define GC_NOTALLITEMS                 -7700114
#define GC_CANTHAVEBOTH                -7700116
#define GC_NOSUBSREM                   -7700118
#define GC_NOTALLSUBST                 -7700120
#define GC_COLLVALCHANGED              -7700122
#define GC_DIFFCALC                    -7700124
#define GC_INVREPOTYPE                 -7700126
#define GC_INCONSISTENTTX              -7700128
#define GC_AFTERTIMELIMIT              -7700130
#define GC_ROTIMELIMIT                 -7700132
#define GC_NOPREWARNING                -7700134
#define GC_TRADENOTALLOCATED           -7700136
#define GC_COLLVALUEDIFFER             -7700138
#define GC_NOTENOUGHPREWARNED          -7700140
#define GC_TOOMANYSEC                  -7700141
#define GC_INVSEGMENT                  -7700142
#define GC_ITEMFAIL                    -7700144
#define GC_SUBNOTALLOWED               -7700146
#define GC_NOPRICETICK                 -7700148
#define GC_MANYTOMANY                  -7700150
#define GC_INVALLOCUNIT                -7700152
#define GC_TRADENOTSELL                -7700154
#define GC_NODEFCOL                    -7700156
#define GC_NOCOLSER                    -7700158
#define GC_ALREADYALLOCATED            -7700160
#define GC_QUANTITYMISMATCH            -7700162
#define GC_NOTEVENALLOCUNIT            -7700164
#define GC_OVERALLOCATION              -7700165
#define GC_INVCOLLATERAL               -7700167
#define GC_COLLHOLIDAY                 -7700168
#define GC_NOSECREMAINING              -7700170
#define GC_USERNOTACTIVE               -7700172
#define GC_INVUSERSTATE                -7700174
#define GC_ISALLOCATED                 -7700176
#define GC_ISANNULLED                  -7700178
#define GC_ISREVERSING                 -7700180
#define GC_ITEMSNINVALID               -7700182
#define GC_NOENOUGHPRW                 -7700184
#define GC_ALRPREWARNED                -7700186
#define GC_ITEMNOTPROCESSED             7700187
#define GC_INVCOLLSERIES               -7700188
#define GC_NOCOLLFORREPO               -7700190
#define GC_NOFSRSUBSREM                -7700192
#define GC_TMUCHQTYPWD                 -7700194
#define GC_AFTERSTOPDATE               -7700196
#define GC_FACILITY_END                 7700197

/******************************************************************************
*end*of* gc_messages.h
*******************************************************************************/

#endif /* _GC_MESSAGES_H_ */

#ifndef _GEN_MESSAGES_H_
#define _GEN_MESSAGES_H_

/******************************************************************************
Module: gen_messages.h

Message definitions generated 2001-10-25 15:45:20 by amt, version 3.1.0.0.

NOTE: Do not edit this file. It is automatically generated from
      one or more message files.
*******************************************************************************/

#define GEN_SHUT_REQ_NEWP               100001
#define GEN_SECFAIL                    -100002
#define GEN_NOPRSENTRY                 -100003
#define GEN_INVCMD                     -100005
#define GEN_AMBIGCMD                   -100007
#define GEN_INVATTRVAL                 -100008
#define GEN_NOATTRVAL                  -100010
#define GEN_TRUNCATTRVAL               -100011
#define GEN_MOREATTRVAL                -100013
#define GEN_NOCOMMAND                  -100014
#define GEN_ERRPARSE                   -100016
#define GEN_MISSENVVAR                 -100018
#define GEN_VALNOTVALID                -100020
#define GEN_MIBFULL                    -100021
#define GEN_CONTINUE_EXECUTION          100023
#define GEN_NOVALUE                    -100025
#define GEN_INVARG                     -100027
#define GEN_CMDTIMEOUT                 -100029
#define GEN_TOOMANYARG                 -100031
#define GEN_NOLOGFILE                  -100033
#define GEN_MISSARG                    -100035
#define GEN_INVARGVAL                  -100037
#define GEN_NODEFFILE                  -100038
#define GEN_TOHIGHINST                 -100040
#define GEN_PRCNOTDEF                  -100042
#define GEN_NOEXCHNAME                 -100044
#define GEN_PRCALRINMIB                -100046
#define GEN_NOCMDTHREAD                -100048
#define GEN_DEFVALTOOLONG              -100050
#define GEN_TRUNC                      -100051
#define GEN_ENVVARNOTDEF                100053
#define GEN_BADPRIORITY                -100054
#define GEN_NOSUBSYSCFG                -100056
#define GEN_FOPENERR                   -100058
#define GEN_LOGFOPENERR                -100060
#define GEN_ENVTOONESTED               -100062
#define GEN_PRCNOTEXIST                -100064
#define GEN_PRCBUSYCMD                 -100066
#define GEN_PRCBADCMDMODE              -100068
#define GEN_NODYNMEM                   -100070
#define GEN_CMDLOST                    -100072
#define GEN_NULLINP                    -100073
#define GEN_DATTIMINV                  -100075
#define GEN_DAYMONINV                  -100077
#define GEN_IMPLNOTEST                 -100079
#define GEN_INTERNALERROR              -100081
#define GEN_INVSEGMENTNR               -100083
#define GEN_INSFMEM                    -100084
#define GEN_BUGCHECK                   -100086
#define GEN_INVTRANSSIZE               -100088
#define GEN_INVTRANSTYPE               -100090
#define GEN_NOTFOU                     -100092
#define GEN_FTP_ERROR                  -100094
#define GEN_FTP_OK                      100095
#define GEN_RT_ERRALLOMEM              -105000
#define GEN_RT_ERRFACNAM               -105002
#define GEN_RT_ERRDATPTYP              -105004
#define GEN_RT_KEYTYPNOTSUP            -105006
#define GEN_RT_PROTOCOLERR             -105008
#define GEN_RT_TRUNCATED               -105010
#define GEN_RT_FLAGOUTRANG             -105012
#define GEN_RT_INVEVTNAM               -105014
#define GEN_RT_NOTALLOW                -105016
#define GEN_RT_ABORT                   -105018
#define GEN_RT_TOOMANYDATP             -105020
#define GEN_RT_FLAGMISS                -105022
#define GEN_RT_INVDATPLEN              -105024
#define GEN_RT_NOTINIT                 -105026
#define GEN_RT_UNKPARNAME              -105028
#define GEN_RT_PARCNT                  -105030
#define GEN_RT_BADPARAM                -105032
#define GEN_RT_INTOVF                  -105034
#define GEN_RT_INVTXNUMBER             -105036
#define GEN_RT_INVTXTYPE               -105038
#define GEN_CDC_OK                      110001
#define GEN_CDC_MEM_FULL               -110002
#define GEN_CDC_MEM_NOTFOU             -110003
#define GEN_CDC_TRC_NOTDEF             -110005
#define GEN_CDC_INS_NOTFOU             -110007
#define GEN_CDC_INT_NOTFOU             -110009
#define GEN_CDC_LIU_NOTFOU             -110011
#define GEN_CDC_LTT_NOTFOU             -110013
#define GEN_CDC_MAR_NOTFOU             -110015
#define GEN_CDC_USR_NOTFOU             -110017
#define GEN_CDC_CLI_NOTFOU             -110019
#define GEN_CDC_VPT_NOTFOU             -110021
#define GEN_CDC_INT_CLOSED             -110023
#define GEN_CDC_ILL_TRT_IN_USR         -110025
#define GEN_CDC_NO_MATCH               -110027
#define GEN_CDC_INT_FULL               -110028
#define GEN_CDC_INS_FULL               -110030
#define GEN_CDC_LTT_FULL               -110032
#define GEN_CDC_LIU_FULL               -110034
#define GEN_CDC_PQR_FULL               -110036
#define GEN_CDC_USR_FULL               -110038
#define GEN_CDC_MAR_FULL               -110040
#define GEN_CDC_VPT_FULL               -110042
#define GEN_CDC_CLI_FULL               -110044
#define GEN_CDC_INT_SIZE_FULL          -110046
#define GEN_CDC_USR_TRT_ID_FULL        -110048
#define GEN_CDC_NOT_INIT               -110049
#define GEN_CDC_NOT_LOCKED             -110051
#define GEN_CDC_NOT_UPDATER            -110053
#define GEN_CDC_LAST_TRADE             -110055
#define GEN_CDC_MAR_LOCKED             -110057
#define GEN_CDC_ILL_COMMODITY          -110059
#define GEN_CDC_CBS_FULL               -110060
#define GEN_CDC_VMS_FULL               -110062
#define GEN_CDC_ICO_FULL               -110064
#define GEN_CDC_USR_NAME_NOTFOU        -110066
#define GEN_CDC_NEAR_MAX_LIMIT         -110067
#define GEN_CDC_TABLE_NOT_FOUND        -110069
#define GEN_CDC_TRT_FULL               -110070
#define GEN_CDC_TRT_NOTFOU             -110071
#define GEN_CDC_PQR_NOTFOU             -110073
#define GEN_CDC_USR_SUSP               -110075
#define GEN_CDC_SYM_FULL               -110076
#define GEN_CDC_SYM_NOTFOU             -110077
#define GEN_CDC_CST_SUSP               -110079
#define GEN_CDC_VOL_FULL               -110080
#define GEN_CDC_NOT_VPT                -110081
#define GEN_CDC_NOT_VPD                -110083
#define GEN_CDC_VPD_FULL               -110084
#define GEN_CDC_VPD_NOTFOU             -110085
#define GEN_CDC_MKT_NOTVAL             -110087
#define GEN_CDC_BBO_NOTFOU             -110089
#define GEN_CDC_COM_NOTFOU             -110091
#define GEN_CDC_CRS_NOTFOU             -110093
#define GEN_CDC_INC_NOTFOU             -110095
#define GEN_CDC_SPR_NOTFOU             -110097
#define GEN_CDC_TRC_NOTFOU             -110099
#define GEN_CDC_BBO_FULL               -110100
#define GEN_CDC_COM_FULL               -110102
#define GEN_CDC_CRS_FULL               -110104
#define GEN_CDC_INC_FULL               -110106
#define GEN_CDC_SPR_FULL               -110108
#define GEN_CDC_TRC_FULL               -110110
#define GEN_CDC_OBL_FULL               -110112
#define GEN_CDC_OBL_NOTFOU             -110113
#define GEN_CDC_MSU_FULL               -110114
#define GEN_CDC_MSU_NOTFOU             -110115
#define GEN_CDC_COG_FULL               -110116
#define GEN_CDC_COG_NOTFOU             -110117
#define GEN_CDC_FML_FULL               -110118
#define GEN_CDC_FML_NOTFOU             -110119
#define GEN_CDC_OBK_FULL               -110120
#define GEN_CDC_OBK_NOTFOU             -110121
#define GEN_CDC_TSR_FULL               -110122
#define GEN_CDC_TSR_NOTFOU             -110123
#define GEN_CDC_EDG_FULL               -110124
#define GEN_CDC_EDG_NOTFOU             -110125
#define GEN_CDC_SPG_FULL               -110126
#define GEN_CDC_SPG_NOTFOU             -110127
#define GEN_CDC_PDP_FULL               -110128
#define GEN_CDC_PDP_NOTFOU             -110129
#define GEN_CDC_FOV_FULL               -110130
#define GEN_CDC_FOV_NOTFOU             -110131
#define GEN_CDC_TSE_FULL               -110132
#define GEN_CDC_TSE_NOTFOU             -110133
#define GEN_CDC_DES_FULL               -110134
#define GEN_CDC_DES_NOTFOU             -110135
#define GEN_CDC_EID_FULL               -110136
#define GEN_CDC_EID_NOTFOU             -110137
#define GEN_CDC_UST_FULL               -110138
#define GEN_CDC_UST_NOTFOU             -110139
#define GEN_CDC_EXC_FULL               -110140
#define GEN_CDC_EXC_NOTFOU             -110141
#define GEN_CDC_VDS_FULL               -110142
#define GEN_CDC_VDS_NOTFOU             -110143
#define GEN_CDC_DGR_FULL               -110144
#define GEN_CDC_DGR_NOTFOU             -110145
#define GEN_CDC_DHB_FULL               -110146
#define GEN_CDC_DHB_NOTFOU             -110147
#define GEN_CDC_SST_FULL               -110148
#define GEN_CDC_SST_NOTFOU             -110149
#define GEN_CDC_PRL_FULL               -110150
#define GEN_CDC_PRL_NOTFOU             -110151
#define GEN_CDC_SERIES_STOPPED         -110153
#define GEN_CDC_INV_BLOCK              -110155
#define GEN_CDC_INV_QUANTITY           -110157
#define GEN_CDC_INV_PREMIUM            -110159
#define GEN_CDC_INV_OPEN_CLOSE_REQ     -110161
#define GEN_CDC_CRSCLOSE               -110163
#define GEN_CDC_INVOPENCLOSE           -110165
#define GEN_CDC_MATCH_VALIDITY         -110167
#define GEN_CDC_INVUSER                -110169
#define GEN_CDC_MAX_QUANTITY           -110171
#define GEN_CDC_MIN_OF_SIL             -110173
#define GEN_CDC_MAR_CLOSED             -110175
#define GEN_CDC_BEFORE_TRD_TIME        -110177
#define GEN_CDC_AFTER_TRD_TIME         -110179
#define GEN_CDC_BEFORE_TX_TIME         -110181
#define GEN_CDC_AFTER_TX_TIME          -110183
#define GEN_CDC_CIB_FULL               -110184
#define GEN_CDC_CIB_NOTFOU             -110185
#define GEN_CDC_SID_FULL               -110186
#define GEN_CDC_SID_NOTFOU             -110187
#define GEN_CDC_NTD_FULL               -110188
#define GEN_CDC_NTD_NOTFOU             -110189
#define GEN_CDC_NO_HEDGE_PRICE         -110191
#define GEN_CDC_CRF_FULL               -110192
#define GEN_CDC_CRF_NOTFOU             -110193
#define GEN_CDC_CBX_FULL               -110194
#define GEN_CDC_CBX_NOTFOU             -110195
#define GEN_CDC_WKP_FULL               -110196
#define GEN_CDC_WKP_NOTFOU             -110197
#define GEN_CDC_MAX_MEM_QUANTITY       -110199
#define GEN_CDC_CRO_FULL               -110200
#define GEN_CDC_CRO_NOTFOU             -110201
#define GEN_CDC_CCR_FULL               -110202
#define GEN_CDC_CCR_NOTFOU             -110203
#define GEN_CDC_IXV_FULL               -110204
#define GEN_CDC_IXV_NOTFOU             -110205
#define GEN_INF_HIT                     115001
#define GEN_INF_NOHIT                  -115002
#define GEN_INF_AMBIG                  -115004
#define GEN_INF_NOFILE                 -115006
#define GEN_INF_WRNBR                  -115008
#define GEN_INF_TOMAN                  -115010
#define GEN_INF_TOFEW                  -115012
#define GEN_SEC_OK                      120001
#define GEN_SEC_NYI                    -120003
#define GEN_SEC_FAIL                   -120004
#define GEN_SEC_INVID                  -120006
#define GEN_SEC_INVIDENT               -120008
#define GEN_SEC_TABFULL                -120010
#define GEN_SEC_FILTOOSMALL            -120012
#define GEN_SEC_SECTOOSMALL            -120014
#define GEN_LCK_OK                      125001
#define GEN_LCK_NYI                    -125003
#define GEN_LCK_INVID                  -125004
#define GEN_LCK_TABFULL                -125006
#define GEN_TIM_CTRY_NOT_FOUND         -130000
#define GEN_TIM_NOT_INIT               -130002
#define GEN_LIST_NAME_NO_MARKET        -135000
#define GEN_LIST_NAME_NO_NAME          -135002
#define GEN_LIST_NAME_NO_TYPE          -135004
#define GEN_LIST_NAME_NOT_AST          -135006
#define GEN_LIST_NAME_TOO_SHORT        -135008
#define GEN_LIST_NAME_DUPL             -135010
#define GEN_LIST_NAME_DUPL_TYP         -135012
#define OC_BIND_FAIL                   -140000
#define OC_BRX_NOT_ALL                 -140001
#define OC_CALLCONN                     140003
#define OC_CLIENT_CONNECT               140005
#define OC_ERROR                       -140006
#define OC_FATAL                       -140008
#define OC_INFO                         140009
#define OC_INVARGS                     -140010
#define OC_INVCHN_TYPE                 -140012
#define OC_INVLEVEL                    -140014
#define OC_INVSRVNAM                   -140016
#define OC_MSG_XMT                      140017
#define OC_NO_EVENTS                    140019
#define OC_NORMAL                       140021
#define OC_PROTOCOLERR                 -140022
#define OC_READ_PRG                     140023
#define OC_SRV_ONLINE                   140025
#define OC_TX_ABORT_INPROG             -140026
#define OC_TX_ALLSRVSHA                -140028
#define OC_TX_ALLSRVSTA                -140030
#define OC_TX_ALRSHADOW                -140032
#define OC_TX_CH_ACTIVE                -140034
#define OC_TX_CH_NOTACTIVE             -140036
#define OC_TX_COMMIT                    140037
#define OC_TX_CONNOTSET                -140038
#define OC_TX_DATTYPDIF                -140040
#define OC_TX_DCLNOTTERM               -140041
#define OC_TX_DELAYTERM                 140043
#define OC_TX_EXINOTCON                -140044
#define OC_TX_INVACCESS                -140046
#define OC_TX_INVFACNAM                -140048
#define OC_TX_INVTXBUFLEN              -140050
#define OC_TX_KEYLENDIF                -140052
#define OC_TX_KEYOFFDIF                -140054
#define OC_TX_KEYOUTREC                -140056
#define OC_TX_KEYRANOVE                -140058
#define OC_TX_MSG_PEND                 -140060
#define OC_TX_NOACCESS                 -140062
#define OC_TX_NOACP                    -140064
#define OC_TX_NONDEQ_MSG                140065
#define OC_TX_NONTRM_READ              -140066
#define OC_TX_NONTRM_SRVCOM            -140067
#define OC_TX_NOSRV_FOR_KEY            -140068
#define OC_TX_NOT_OPEN                 -140070
#define OC_TX_NOTACTIVE                -140072
#define OC_TX_NOTSAMCLU                -140074
#define OC_TX_NOTSAMNOD                -140076
#define OC_TX_NRSEGSDIF                -140078
#define OC_TX_OUTST_EVTSDWN            -140080
#define OC_TX_REQABORT                 -140082
#define OC_TX_REQCALLUN                -140084
#define OC_TX_SHUTCALL                 -140085
#define OC_TX_SHUTDOWN                 -140087
#define OC_TX_START                     140089
#define OC_TX_STARTREC                  140091
#define OC_TX_TOOMANSHA                -140092
#define OC_TX_TRUNCATE                 -140094
#define OC_TX_TWONODALR                -140096
#define OC_TX_UPPLESLOW                -140098
#define OC_TX_VOTE                      140099
#define OC_TRNS_UNSUPP                 -140100
#define OC_TX_JOUOVERFL                -140102
#define OC_TX_JOUFILFUL                -140104
#define OC_TX_JOUFILERR                -140106
#define OMNI_SYNCHPOINT                -145000
#define OMNI_SECURITY                  -145002
#define OMNI_LOGIN_FAIL                -145004
#define OMNI_NOT_NOTIFIED              -145005
#define OMNI_DUPL_LOGIN                -145006
#define OMNI_UNKNOWNBYBE               -145008
#define OMNI_OAPIOLD                   -145010
#define OMNI_TIMED_OUT                 -145011
#define MXABS_OK                        150001
#define MXABS_INVHANDLE                -150003
#define MXABS_INVLEN                   -150005
#define MXABS_NOTFND                   -150007
#define MXABS_NYI                      -150009
#define MXABS_NOOP4CLIENT              -150011
#define MXABS_NOACCESS                 -150013
#define MXABS_NOACTION                 -150015
#define MXABS_NOREPORT                 -150017
#define MXABS_ALROPEN                  -150019
#define MXABS_INVEXERPT                -150021
#define MXABS_SUBJ2DLL                 -150023
#define MXABS_INVTXCODE                -150024
#define MXABS_BADQTY4REM               -150025
#define MXABS_SYNCINPROG               -150027
#define MXABS_DLLINPROG                -150029
#define MXABS_ALRRES                   -150031
#define MXABS_NOTRES                   -150033
#define MXABS_TRADERSUSP               -150035
#define MXABS_CXLREPLMENT              -150037
#define MXABS_OPMISMATCH               -150039
#define MXABS_ALERT                    -150041
#define MXAOF_INTERR                   -155000
#define MXAOF_COMERR                   -155002
#define MXAOF_NOLOG                    -155004
#define MXAOF_MSG_REROUTED             -155005
#define MXAOF_MSG_IGNORED              -155006
#define MXAOF_SEQ_NO_TO_AOF             155007
#define MXAOF_SEQ_NO_FROM_AOF           155009
#define MXFLT_INTERR                   -160000
#define MXFLT_COMERR                   -160002
#define MXFLT_NOLOG                    -160004
#define MXFLT_DATAERR                  -160006
#define MXFLT_HANDLERR                 -160008
#define MXFLT_NOTSPEC                  -160010
#define MXFLT_NOTFOUND                 -160012
#define MXFLT_FILTERR                  -160014
#define GEN_RTR_ERRALLOMEM             -165000
#define GEN_RTR_ERRFACNAM              -165002
#define GEN_RTR_ERRDATPTYP             -165004
#define GEN_RTR_KEYTYPNOTSUP           -165006
#define GEN_RTR_PROTOCOLERR            -165008
#define GEN_RTR_TRUNCATED              -165010
#define GEN_RTR_FLAGOUTRANG            -165012
#define GEN_RTR_INVEVTNAM              -165014
#define GEN_RTR_NOTALLOW               -165016
#define GEN_RTR_ABORT                  -165018
#define GEN_RTR_TOOMANYDATP            -165020
#define GEN_RTR_FLAGMISS               -165022
#define GEN_RTR_INVDATPLEN             -165024
#define GEN_RTR_NOTINIT                -165026
#define GEN_RTR_UNKPARNAME             -165028
#define GEN_RTR_PARCNT                 -165030
#define GEN_RTR_BADPARAM               -165032
#define GEN_RTR_INTOVF                 -165034
#define GEN_RTR_INVTXNUMBER            -165036
#define GEN_RTR_INVTXTYPE              -165038
#define GEN_RTR_SRVABOTX               -165040
#define GEN_RTR_NOSRVAVL                165041
#define GEN_RTR_INVFLAGS               -165042
#define GEN_RTR_INVCHANNEL             -165044
#define GEN_RTR_INVFACNAM              -165046
#define GEN_RTR_FAILED                 -165048
#define GEN_TPS_NOHOST                 -166000
#define GEN_TPS_ERRLOCLNAME            -166002
#define GEN_TPS_BADIPADDR              -166004
#define GEN_TPS_ERRHOSTNAME            -166006
#define GEN_TPS_ERROPENSOCK            -166008
#define GEN_TPS_ERRTHREAD              -166010
#define GEN_TPS_ERRTCPCONN             -166012
#define GEN_TPS_INVHANDLE              -166014
#define GEN_TPS_INVBUFFER              -166016
#define GEN_TPS_NOCALLBACK             -166018
#define GEN_TPS_INSMEM                 -166020
#define GEN_TPS_COMMERR                -166022
#define GEN_TPS_INSBUFF                -166024
#define GEN_TPS_INVSEM                 -166026
#define GEN_TPS_ERRBIND                -166028
#define GEN_TPS_ERRLSTN                -166030
#define GEN_TPS_INVOCVER               -166032
#define GEN_TPS_INVMODE                -166034
#define GEN_TPS_EOF                    -166036

/******************************************************************************
*end*of* gen_messages.h
*******************************************************************************/

#endif /* _GEN_MESSAGES_H_ */
#ifndef _IMP_MESSAGES_H_
#define _IMP_MESSAGES_H_

/******************************************************************************
Module: imp_messages.h

Message definitions generated 2001-10-01 13:16:21 by amt, version V2.4.

NOTE: Do not edit this file. It is automatically generated from
      one or more message files.
*******************************************************************************/

#define IMP_COUNTERS_NOT_INIT          -1905000
#define IMP_INFORMATION                 1905001
#define IMP_SUCCESS                     1905003
#define IMP_PANIC                      -1905004
#define IMP_BUGCHECK                   -1905005
#define IMP_QUOTA                      -1905007
#define IMP_INSFMEM                    -1905008
#define IMP_LISEMPTY                   -1905009
#define IMP_ABORT                      -1905010
#define IMP_IVBUFLEN                   -1905012
#define IMP_LOG_ALREADY_INIT            1910001
#define IMP_LOG_NOT_INITIALIZED        -1910002
#define IMP_LOG_NOT_READ_MODE          -1910004
#define IMP_LOG_NOT_WRITE_MODE         -1910006
#define IMP_LOG_END_OF_FILE            -1910007
#define IMP_LOG_UNDEFINED_MODE         -1910008
#define IMP_LOG_TOO_SMALL              -1910010
#define IMP_LOG_WRONG_SIZE             -1910012
#define IMP_LOG_UNKNOWN                -1910014
#define IMP_LOG_COMMIT_LOG_UNKNOWN     -1910016
#define IMP_OB_INV_HANDLE              -1915000
#define IMP_OB_NO_RESOURCE             -1915002
#define IMP_OB_NOT_COMMODITY           -1915003
#define IMP_OB_NOT_COSTUMER            -1915005
#define IMP_OB_NOT_INIT                -1915006
#define IMP_OB_NOT_LOCKED              -1915008
#define IMP_OB_NOT_SERIES              -1915009
#define IMP_OB_OLD_COMMODITY           -1915011
#define IMP_OB_OLD_CUSTOMER            -1915013
#define IMP_OB_OLD_SERIES              -1915015
#define IMP_OB_END                      1915017
#define IMP_OB_TENTATIVE                1915019
#define IMP_OB_NOT_CUSTOMER            -1915021
#define IMP_QRY_INTERRUPTED            -1920001
#define IMP_QRY_NOT_OWN                -1920003
#define IMP_QRY_ORDER_NO_BUT_NOT_SIDE  -1920005
#define IMP_QRY_SIDE_BUT_NOT_ORDER_NO  -1920007
#define IMP_QRY_ONO_WITHOUT_SERIES     -1920009
#define IMP_QRY_TOO_WIDE_SERIES        -1920011
#define IMP_QRY_ONO_WITHOUT_COMMODITY  -1920013
#define IMP_QRY_INVALID_VALUE_O_SEL    -1920015
#define IMP_MATCH_INVALID_SIZE         -1925001
#define IMP_MATCH_SYSTEM               -1925003
#define IMP_MATCH_MKT_OR_LMT           -1925005
#define IMP_MATCH_IGNORE_FLAG          -1925007
#define IMP_MATCH_CLIENT_CAT           -1925009
#define IMP_MATCH_REQUOTED_TWICE       -1925011
#define IMP_MATCH_CROSS_QUOTE          -1925013
#define IMP_MATCH_OWN_BIN_REQ          -1925015
#define IMP_MATCH_CAB_NOT_ALLOW        -1925017
#define IMP_MATCH_INT_MKT_O_LFT         1925019
#define IMP_MATCH_ALTER_NOT_ALLOW      -1925021
#define IMP_MATCH_QTY_INC_NOT_ALLOW    -1925023
#define IMP_MATCH_MM_CANNOT_ALTER      -1925025
#define IMP_MATCH_QUANTITY             -1925027
#define IMP_MATCH_LOW_VOLUME           -1925029
#define IMP_MATCH_PREMIUM              -1925031
#define IMP_MATCH_STOPPED              -1925033
#define IMP_MATCH_BID_ASK              -1925035
#define IMP_MATCH_VALIDITY             -1925037
#define IMP_MATCH_MKT_ORDER_PRICE      -1925039
#define IMP_MATCH_OVERBID              -1925041
#define IMP_MATCH_INV_ALTER            -1925043
#define IMP_MATCH_MARBOUNCE            -1925045
#define IMP_MATCH_SER_IS_EXPIRED       -1925047
#define IMP_MATCH_NO_USER_SPECIFIED    -1925049
#define IMP_MATCH_ONLY_DELETE_OWN_CUST -1925051
#define IMP_MATCH_LOWEST_TICK          -1925053
#define IMP_MATCH_NOT_NUF_STEP_UP_VOL  -1925055
#define IMP_MATCH_WC_DEL_SPEC_ONO      -1925057
#define IMP_MATCH_WC_DEL_SPEC_SIDE     -1925059
#define IMP_MATCH_DEL_DERIVED_ORDERS   -1925061
#define IMP_MATCH_TOO_SMALL_QUOTE      -1925063
#define IMP_MATCH_NOT_MARKET_MAKER     -1925065
#define IMP_MATCH_NOT_PMM              -1925067
#define IMP_MATCH_WRONG_ITEM_NUMBER    -1925069
#define IMP_MATCH_QUOTES_DELETED        1925071
#define IMP_MATCH_ROTATION_CROSS        1925073
#define IMP_MATCH_ROTATION_SIDE_EMPTY   1925075
#define IMP_MATCH_STATUS_NOT_ALLOWED   -1925077
#define IMP_MATCH_ROTATION_NOT_PMM     -1925079
#define IMP_MATCH_ROT_WRONG_STATUS     -1925081
#define IMP_MATCH_BLOCK_FAC_BUSY       -1925083
#define IMP_MATCH_NO_SUCH_ORDER        -1925085
#define IMP_MATCH_WRONG_OID            -1925087
#define IMP_MATCH_NOT_FAC              -1925089
#define IMP_MATCH_NOT_BLOCK            -1925091
#define IMP_MATCH_BLOCK_FAC_QUANTITY   -1925093
#define IMP_MATCH_BLOCK_FAC_INV_QNTY   -1925095
#define IMP_MATCH_INV_OPEN_CLOSE       -1925097
#define IMP_MATCH_INV_EAM_OPEN_CLOSE   -1925099
#define IMP_MATCH_NOT_OWN_USER         -1925101
#define IMP_MATCH_NOT_REGULAR          -1925103
#define IMP_MATCH_NOT_EAM              -1925105
#define IMP_MATCH_MUST_BE_LIMIT        -1925107
#define IMP_MATCH_MKT_IOC_OR_FOK       -1925109
#define IMP_MATCH_FAC_BLK_NOT_BEST     -1925111
#define IMP_MATCH_MM_ORDER_NOT_LIMIT   -1925113
#define IMP_MATCH_TOO_LARGE_MKT_ORDER  -1925115
#define IMP_MATCH_ONLY_EAM_CUST_FOK    -1925117
#define IMP_MATCH_PMM_TRADE_REPORT     -1925119
#define IMP_MATCH_ORDER_NOT_FOUND      -1925121
#define IMP_MATCH_TOO_LARGE_VOLUME     -1925123
#define IMP_MATCH_INFERIOR_PRICE       -1925125
#define IMP_MATCH_INTERNAL_TRADE       -1925127
#define IMP_MATCH_INVALID_LOCK         -1925129
#define IMP_MATCH_NO_FARMM_BLOCK       -1925131
#define IMP_MATCH_INV_ROT_TEST_FLAG    -1925133
#define IMP_MATCH_OB_US_NOT_SERIES     -1925135
#define IMP_MATCH_INT_STS_NOT_ALLOW    -1925137
#define IMP_MATCH_INT_INV_SEG_NO       -1925139
#define IMP_MATCH_NOT_ALL_PARAMS       -1925141
#define IMP_MATCH_NO_PMM_PARAMS        -1925143
#define IMP_MATCH_TOO_SMALL_OPEN_VOL   -1925145
#define IMP_MATCH_TOO_SMALL_BEST_VOL   -1925147
#define IMP_MATCH_UNDERLYING_NOT_OPEN  -1925149
#define IMP_MATCH_ONLY_CUST_MKT_ORDER  -1925151
#define IMP_MATCH_NO_PMM_QUOTE         -1925153
#define IMP_MATCH_OPEN_QUANTITY        -1925155
#define IMP_MATCH_TOO_MUCH_CROSS       -1925157
#define IMP_MATCH_ONLY_REGULAR_FOK     -1925159
#define IMP_MATCH_FAC_RESP_PREMIUM     -1925161
#define IMP_MATCH_ILL_ORDER_CAT        -1925163
#define IMP_MATCH_CROSS_OWN_QUOTE      -1925165
#define IMP_MATCH_INACTIVE_ORDER       -1925167
#define IMP_MATCH_LAST_LOGOUT           1925169
#define IMP_MATCH_CANNOT_UNLOCK         1925171
#define IMP_MATCH_ORDER_CAT_VER_FAIL   -1925173
#define IMP_MATCH_ORDER_CAT_FORBID     -1925175
#define IMP_MATCH_NOT_LOCKED           -1925177
#define IMP_MATCH_FM_LVL_TOO_LOW       -1925179
#define IMP_MATCH_WRONG_SIDE           -1925181
#define IMP_MATCH_NO_SUCH_STATUS       -1925183
#define IMP_MATCH_FARMM_NOT_GTC        -1925185
#define IMP_MATCH_IGN_AWAY_MKT_NOT_SET -1925187
#define IMP_MATCH_WC_DEL_NOT_ALLOWED   -1925189
#define IMP_MATCH_INV_LOGB_FILENAME    -1925191
#define IMP_MATCH_MKT_MUST_BE_IOC      -1925193
#define IMP_MATCH_TOO_LARGE_VALUE      -1925195
#define IMP_MATCH_TOO_SMALL_VALUE      -1925197
#define IMP_MATCH_OPEN_QUANTITY_ZERO   -1925199
#define IMP_MATCH_NOT_VALID_ACCOUNT    -1925201
#define IMP_MATCH_REENTERED_TWICE      -1925203
#define IMP_MATCH_CROSS_OB_QUOTE       -1925205
#define IMP_MATCH_CROSS_DE_BBO         -1925207
#define IMP_MATCH_MM_ONLY_IOC          -1925209
#define IMP_MATCH_PIQ_NO_MULTI         -1925211
#define IMP_MATCH_IGNORE_FLAG_JWX      -1925213
#define IMP_MATCH_PIQ_ONLY_ZERO        -1925215
#define IMP_MATCH_ENT_ONLY_SELL        -1925217
#define IMP_MATCH_ENT_ONLY_BUY         -1925219
#define IMP_MATCH_NOT_TRADING          -1925221
#define IMP_MATCH_HEDGE_NOT_SENT       -1925222
#define IMP_MATCH_ILL_INTEREST_FLAG    -1925223
#define IMP_MATCH_INV_MULTIPLIER       -1925225
#define IMP_MATCH_INV_SER_STATUS       -1925227
#define IMP_MATCH_NOT_SAME_COMMODITY   -1925229
#define IMP_MATCH_NOT_MARKET           -1925231
#define IMP_MATCH_SAME_SERIES          -1925233
#define IMP_MATCH_ILL_STEP_UP_FLAG     -1925235
#define IMP_MATCH_INTEREST_ORDER       -1925237
#define IMP_MATCH_SERIES_MISMATCH      -1925239
#define IMP_MATCH_NO_VALID_BBO         -1925241
#define IMP_MATCH_INV_CBO_PRM          -1925243
#define IMP_MATCH_LOW_HIT_VOL          -1925245
#define IMP_MATCH_INV_LEG_NO           -1925247
#define IMP_MATCH_MESSAGE              -1925249
#define IMP_MATCH_NOT_CBO_QNTY         -1925251
#define IMP_MATCH_INV_ACTION           -1925253
#define IMP_MATCH_INV_QNTY_COND        -1925255
#define IMP_MATCH_INV_ALTER_COMMODITY  -1925257
#define IMP_CSR_CHNFAIL                -1930000
#define IMP_CSR_CBDUP                  -1930002
#define IMP_CSR_NOCHNT                 -1930004
#define IMP_CSR_BXTOOLARGE             -1930006
#define IMP_CSR_BXNOIC                 -1930008
#define IMP_CSR_INVCHNTYP              -1930010
#define IMP_CSR_INVTMO                 -1930012
#define IMP_PRM_INVHDL                 -1935000
#define IMP_PRM_PRMNOTFND              -1935002
#define IMP_PRM_PRMBFUL                -1935004
#define IMP_PRM_INVSPG                 -1935006
#define IMP_PRM_INVEXG                 -1935008
#define IMP_PRM_NOPRM                  -1935010
#define IMP_PRM_NOTSET                 -1935012
#define IMP_PRM_SPGERR                 -1935014

/******************************************************************************
*end*of* imp_messages.h
*******************************************************************************/

#endif /* _IMP_MESSAGES_H_ */
#ifndef _IN_MESSAGES_H_
#define _IN_MESSAGES_H_

/******************************************************************************
Module: in_messages.h

Message definitions generated 2001-10-19 08:22:47 by amt, version V2.4.

NOTE: Do not edit this file. It is automatically generated from
      one or more message files.
*******************************************************************************/

#define IDL_BSSDATANOTFOU              -505000
#define IDL_BSSNOTFOU                  -505002
#define IDL_BSSTABFULL                 -505004
#define IDL_COMDATANOTFOU              -505006
#define IDL_COMNOTFOU                  -505008
#define IDL_COMTABFULL                 -505010
#define IDL_EXCDATANOTFOU              -505012
#define IDL_EXCNOTFOU                  -505014
#define IDL_EXCTABFULL                 -505016
#define IDL_INCDATANOTFOU              -505018
#define IDL_INCNOTFOU                  -505020
#define IDL_INCTABFULL                 -505022
#define IDL_INSDATANOTFOU              -505024
#define IDL_INSNOTFOU                  -505026
#define IDL_INSTABFULL                 -505028
#define IDL_INTDATANOTFOU              -505030
#define IDL_INTNOTFOU                  -505032
#define IDL_INTTABFULL                 -505034
#define IDL_MARDATANOTFOU              -505036
#define IDL_MARNOTFOU                  -505038
#define IDL_MARTABFULL                 -505040
#define IDL_SYMDATANOTFOU              -505042
#define IDL_SYMNOTFOU                  -505044
#define IDL_SYMTABFULL                 -505046
#define IDL_TSSDATANOTFOU              -505048
#define IDL_TSSNOTFOU                  -505050
#define IDL_TSSTABFULL                 -505052
#define IDL_VMSDATANOTFOU              -505054
#define IDL_VMSNOTFOU                  -505056
#define IDL_VMSTABFULL                 -505058
#define IDL_VOLDATANOTFOU              -505060
#define IDL_VOLNOTFOU                  -505062
#define IDL_VOLTABFULL                 -505064
#define IDL_BBODATANOTFOU              -505066
#define IDL_BBONOTFOU                  -505068
#define IDL_BBOTABFULL                 -505070
#define IDL_TRCDATANOTFOU              -505072
#define IDL_TRCNOTFOU                  -505074
#define IDL_TRCTABFULL                 -505076
#define IDL_OBKDATANOTFOU              -505078
#define IDL_OBKNOTFOU                  -505080
#define IDL_OBKTABFULL                 -505082
#define IDL_AEXDATANOTFOU              -505084
#define IDL_AEXNOTFOU                  -505086
#define IDL_AEXTABFULL                 -505088
#define IDL_TSEDATANOTFOU              -505090
#define IDL_TSENOTFOU                  -505092
#define IDL_TSETABFULL                 -505094
#define IDL_SSTDATANOTFOU              -505096
#define IDL_SSTNOTFOU                  -505098
#define IDL_SSTTABFULL                 -505100
#define IDL_PTDDATANOTFOU              -505102
#define IDL_PTDNOTFOU                  -505104
#define IDL_PTDTABFULL                 -505106
#define IDL_IPRDATANOTFOU              -505108
#define IDL_IPRNOTFOU                  -505110
#define IDL_IPRTABFULL                 -505112
#define IDL_EIDDATANOTFOU              -505114
#define IDL_EIDNOTFOU                  -505116
#define IDL_EIDTABFULL                 -505118
#define IDL_DHBDATANOTFOU              -505120
#define IDL_DHBNOTFOU                  -505122
#define IDL_DHBTABFULL                 -505124
#define IDL_DESDATANOTFOU              -505126
#define IDL_DESNOTFOU                  -505128
#define IDL_DESTABFULL                 -505130
#define IDL_DGRDATANOTFOU              -505132
#define IDL_DGRNOTFOU                  -505134
#define IDL_DGRTABFULL                 -505136
#define IDL_VDSDATANOTFOU              -505138
#define IDL_VDSNOTFOU                  -505140
#define IDL_VDSTABFULL                 -505142
#define IDL_MEMDATANOTFOU              -505144
#define IDL_MEMNOTFOU                  -505146
#define IDL_MEMTABFULL                 -505148
#define IDL_CBXDATANOTFOU              -505150
#define IDL_CBXNOTFOU                  -505152
#define IDL_CBXTABFULL                 -505154
#define IDL_PSPDATANOTFOU              -505156
#define IDL_PSPNOTFOU                  -505158
#define IDL_PSPTABFULL                 -505160
#define IDL_USRDATANOTFOU              -505162
#define IDL_USRNOTFOU                  -505164
#define IDL_USRTABFULL                 -505166
#define IDL_CBSDATANOTFOU              -505168
#define IDL_CBSNOTFOU                  -505170
#define IDL_CBSTABFULL                 -505172
#define IDL_SBSDATANOTFOU              -505174
#define IDL_SBSNOTFOU                  -505176
#define IDL_SBSTABFULL                 -505178
#define IDL_CORRUPTED                  -505180
#define IDL_CONSISTENT                  505181
#define IDL_WRONGLCK                   -505182
#define IDL_CCRDATANOTFOU              -505184
#define IDL_CCRNOTFOU                  -505186
#define IDL_CCRTABFULL                 -505188
#define INFO_OPERATORLOG               -510000
#define INFO_RESTART_OPERATION          510001
#define INFO_SXBUFFUL                  -510002
#define INFO_SXCSUMERR                 -510004
#define INFO_SXINVEXCCODE              -510006
#define INFO_SXMSGLENERR               -510008
#define INFO_SXMSGUNK                  -510010
#define INFO_SXNOOMX                   -510012
#define INFO_SXRTRYEXH                 -510014
#define INFO_BADTRDSYNCH               -510016
#define INFO_NOTRDSYNCH                -510018
#define INFO_BADORDSYNCH               -510020
#define INFO_INITNOTPASSED             -510022
#define INFO_PRIMTOSOON                -510024
#define INFO_NOTONETOONEMP             -510026
#define INFO_PANIC                     -510028
#define INFO_SERIESNOTFOU              -510030
#define INFO_PRIMARY                    510031
#define INFO_WAITPRIM                   510033
#define INFO_PRIMFOUND                  510035
#define INFO_MODDEPTH                   510037
#define INFO_NOMODDEPTH                 510039
#define INFO_NEWDEAL                    510041
#define INFO_NEWTRADE                   510043
#define INFO_DELDEAL                    510045
#define INFO_SKIPDEAL                   510047
#define INFO_SKIPORDER                  510049
#define INFO_TRDSYNCH                   510051
#define INFO_ORDSYNCH                   510053
#define INFO_DEALNOTFOU                -510055
#define INFO_DUPDEAL                   -510057
#define INFO_DUPORDER                  -510059
#define INFO_ORDERNOTFOU               -510061
#define INFO_SPREADUNSAFE              -510063
#define INFO_INDEXDUNSAFE              -510065
#define INFO_OK                         510067
#define INFO_NYI                        510069
#define INFO_BUGCHECK                  -510070
#define INFO_INSFMEM                   -510072
#define INFO_ABORT                     -510074
#define INFO_SUCCESS                    510075
#define INFO_NOINFO                    -510077
#define INFO_TODAYNOTAVAIL             -510079
#define INFO_BADSEG                    -510080
#define INFO_INVDATEFORM               -510082
#define INFO_INVFROMTODATE             -510084
#define INFO_NOTINMEMDB                 510085
#define INFO_NOINFOUV                  -510087
#define INFO_NOINFOINS                 -510089
#define INFO_NODEFSERVICES             -510091
#define INFO_MARGINQUOTESUPD           -510093
#define INFO_MARGINQUOTESVIEWED        -510095
#define INFO_IVBUFLEN                  -510096
#define INFO_FILENOTFOUND              -510098
#define INFO_NO_DATA_RETRY             -510099
#define INFO_FORCEDROLL                -510100
#define INFO_DB_RETRY_EXHAUSTED        -510102
#define INFO_TRAVERSEND                -510104
#define INFO_RTRERROR                  -510106
#define INFO_PRIMGROUPSHUT             -510108
#define INFO_COMPLETE                   510109
#define INFO_SSLNOCLOSE                -510111
#define INFO_SSLWVERISON               -510112
#define INFO_SSLNOLOGFILE              -510113
#define INFO_SSLNOTLOGENA              -510115
#define INFO_SSLNOMOUNT                -510116
#define INFO_SSLNOCHANNEL              -510117
#define INFO_SSLCALLBCKREG             -510118
#define INFO_SSLRICNOTOPEN             -510119
#define INFO_SSLNODISPATCH             -510120
#define INFO_SSLDISCONNECT             -510122
#define INFO_SSLRECONNECT               510123
#define INFO_SSLRICICLOSED              510125
#define INFO_SSLRICINFO                 510127
#define INFO_SSLRICOK                   510129
#define INFO_SSLRICSTALE               -510131
#define INFO_SSLEVUNK                   510133
#define INFO_SSLDISMOUNT               -510135
#define INFO_SSLNOWALIDD               -510136
#define INFO_NOTAWAYMARKET             -510138
#define INFO_NOSERIESTERM               510139
#define INFO_OPERNOTACC                -510140
#define INFO_OPERNYI                   -510142
#define INFO_UNKNOWNOPER               -510144
#define INFO_NEWFIRSTDEAL               510145
#define INFO_DELFIRSTDEAL               510147
#define INFO_DELLASTDEAL                510149
#define INFO_NOTALLOCATED              -510150
#define INFO_NOHOLDBACK                 510151
#define INFO_RETRANSNOTHAND            -510153
#define INFO_ONEINVUNDERLYING          -510155
#define INFO_MODULENOTINIT             -510156
#define INFO_PTDNOTUNIQUE              -510157
#define INFO_INCNOTFOUND               -510159
#define INFO_NOREFTOTIMER              -510161
#define INFO_MODBEST                    510163
#define INFO_CINTNOTFOU                 510165
#define INFO_INSNOTFOU                  510167
#define INFO_BO31NOTADDED               510169
#define INFO_BO35NOTADDED               510171
#define INFO_BD2NOTADDED                510173
#define INFO_MISSINGTIMERREF            510175
#define INFO_HBINTTOSHORT               510177
#define INFO_IPRNOTFOU                 -510178
#define INFO_BO3XNOTADDED               510179
#define INFO_INSNOTADDED                510181
#define INFO_CINTNOTADDED               510183
#define INFO_NOHDLYIELDDIFF            -510184
#define INFO_FUNCNOTSUP                -510186
#define INFO_HBBDCLATE                 -510188
#define INFO_ABORTSTATEREADY           -510190
#define INFO_ABORTNOMATCH              -510192
#define INFO_DATALCKBYSIGNAL           -510194
#define INFO_NOVALUNDERLYING           -510195
#define INFO_ONEINSNOOFFPRICE          -510196
#define INFO_ILLORDERTYPE               510197
#define INFO_PATCHNODEALS              -510198
#define INFO_PATCHDEALGONE             -510200
#define INFO_PATCHVOLERROR             -510202
#define INFO_DUALOPRALINEUSE            510203
#define INFO_TSISCOMBO                  510205
#define PMI_GENRSPABN                  -515000
#define PMI_MALLOCERR                  -515002
#define PMI_LISTRSPABN                 -515004
#define PMI_INITRSPABN                 -515006
#define PMI_NOFREEBLOCK                -515008
#define PMI_NOINCFOUND                 -515010
#define PMI_NOINSFOUND                 -515012
#define PMI_NOUNDFOUND                 -515014
#define PMI_PMISTSABN                  -515015
#define PMI_QANSWOVERFLOW              -515016
#define INREU_INVMSGFMT                -520000
#define INREU_JOKE                     -520002
#define INREU_CFGCMDLNG                -520004
#define INREU_NOCFGFIL                 -520006
#define INREU_NOCRSREF                 -520008
#define INREU_NORICDEF                 -520010
#define INREU_NOPARENTDE               -520012
#define INREU_NOCOMCODDEF              -520014
#define INREU_INVBSTDEF                -520016
#define INREU_ONEBSTDEF                -520018
#define INREU_OMXRECDEF                -520020
#define INREU_NOOMXREC                 -520022
#define INREU_OMXNOTDEF                -520024
#define INREU_OMXNOFLD                 -520026
#define INREU_OMXDEF                   -520028
#define INREU_OMXONEFLD                -520030
#define INREU_OMXOVRLAP                -520032
#define INREU_OMXDBLFLDNO              -520034
#define INREU_INVBDXTYP                -520036
#define INREU_CONNFAIL                 -520038
#define INREU_DCLRCF                   -520040
#define INREU_DCLRTF                   -520042
#define INREU_ISINLEN                  -520044
#define INREU_INVSTSMSG                -520046
#define INREU_ASYINTERR                -520048
#define INREU_ASYDEVACT                -520050
#define INREU_ASYNOVLDCNTX             -520052
#define INREU_ASYNOSTOP                -520054
#define INREU_INVEXPFLD                -520056
#define INREU_INVINSTTYPFLD            -520058
#define INREU_INVTIMFLD                -520060
#define INREU_INVDATFLD                -520062
#define INREU_INVRECSIZ                -520064
#define INREU_BDXNOTDEF                -520066
#define INREU_REUSTS                   -520068
#define INREU_REUPROTERR               -520070
#define INREU_INVCRF                   -520072
#define INREU_NOSERIEDEF               -520074
#define INREU_NOINDEXDEF               -520076
#define INREU_NORMAL                    520077
#define INREU_WRNGSEQNBR               -520079
#define OPRA_CACH_INS_FULL             -525000
#define OPRA_CACH_CINT_FULL            -525002
#define OPRA_CACH_GRP_FULL             -525004
#define OPRA_CACH_EXC_FULL             -525006
#define OPRA_CACH_INS_NOT_RDY          -525008
#define OPRA_CACH_CINT_NOT_RDY         -525010
#define OPRA_CACH_GROUP_NOT_RDY        -525012
#define OPRA_CACH_EXC_NOT_RDY          -525014
#define OPRA_CACH_INSNOTFOU            -525016
#define IN_CQATS_FILEOPRFAILED         -530001
#define IN_CQATS_NORMAL                 530003
#define IN_CQATS_FEED_UNRELIABLE       -530004
#define IN_CQATS_NEW_UNKNOWN_MESSAGE    530005
#define IN_CQATS_UNEXP_SEQUENCE_NUMBER -530007
#define IN_CQATS_CORRECTION_FAILURE    -530009
#define IN_CQATS_CANCEL_FAILURE        -530011
#define IN_ULFEED_FILEOPRFAILED        -535001
#define IN_ULFEED_NORMAL                535003
#define IN_ULFEED_FEED_UNRELIABLE      -535004
#define IN_ULFEED_NEW_UNK_MESSAGE       535005
#define IN_ULFEED_UNEXP_SEQ_NUMBER     -535007
#define IN_ULFEED_CORRECTION_FAILURE   -535009
#define IN_ULFEED_CANCEL_FAILURE       -535011

/******************************************************************************
*end*of* in_messages.h
*******************************************************************************/

#endif /* _IN_MESSAGES_H_ */
#ifndef _LM_MESSAGES_H_
#define _LM_MESSAGES_H_

/******************************************************************************
Module: lm_messages.h

Message definitions generated 2001-06-05 13:00:46 by amt, version V2.4.

NOTE: Do not edit this file. It is automatically generated from
      one or more message files.
*******************************************************************************/

#define LM_PANIC                       -600000
#define LM_SUCCESS                      600001
#define LM_INFORMATION                  600003
#define LM_INVTRANSSIZE                -600004
#define LM_INVTRANSTYPE                -600006
#define LM_INTERNALERROR               -600007
#define LM_INVTX                       -600009
#define LM_INVSEGMENTNR                -600011
#define LM_INSFMEM                     -600012
#define LM_BUGCHECK                    -600014
#define LM_ABORT                       -600016
#define LM_SUP_EXINSID_HASH            -600018
#define LM_SUP_INSID_FULL              -600020
#define LM_SUP_INV_INSID_KEY           -600022
#define LM_SUP_INV_LST_KEY             -600024
#define LM_SUP_INV_MM_KEY              -600026
#define LM_SUP_ORDERTYPE               -600028
#define LM_SUP_INV_PQR_KEY             -600030
#define LM_SUP_INV_PRM_KEY             -600032
#define LM_SUP_LST_FULL                -600034
#define LM_SUP_MM_FULL                 -600036
#define LM_SUP_NOT_OPEN                -600038
#define LM_SUP_PQR_FULL                -600040
#define LM_SUP_PRM_FULL                -600042
#define LM_SUP_SPREAD_NOTFND           -600044
#define LM_SUP_COMM_NOTFND             -600045
#define LM_SUP_INSID_NOTFND            -600047
#define LM_SUP_NO_INSID                -600049
#define LM_SUP_NO_LST_ID               -600051
#define LM_SUP_NOT_INIT                -600053
#define LM_SUP_NOT_LOCKED              -600055
#define LM_SUP_PQR_NOTFND              -600057
#define LM_SUP_PQR_ALTERED              600059
#define LM_SUP_LST_ALTERED              600061
#define LM_SUP_VOL_ALTERED              600063
#define LM_SUP_NOT_UPDATER             -600065
#define LM_SUP_END_OF_LIST             -600067
#define LM_SUP_NO_RESP_MM              -600069
#define LM_SUP_NO_VOL_ID               -600071
#define LM_SUP_MM_NOTRESP              -600073
#define LM_SUP_OLD_INSID               -600075
#define LM_SUP_BID_PRI_MISS            -600077
#define LM_SUP_ASK_PRI_MISS            -600079
#define LM_SUP_BOTH_PRI_MISS           -600081
#define LM_SUP_BID_QTY_INV             -600083
#define LM_SUP_ASK_QTY_INV             -600085
#define LM_SUP_BOTH_QTY_INV            -600087
#define LM_SUP_INV_SPREAD              -600089
#define LM_SUP_INV_VOL_SPR             -600091
#define LM_SUP_OLD_ORDER               -600093
#define LM_SUP_PRM_NOTFND              -600095
#define LM_SUP_INV_VOL_KEY             -600096
#define LM_SUP_VOL_FULL                -600098
#define LM_SUP_VSPREAD_NOTFND          -600100
#define LM_SUP_INV_MEMBER              -600102
#define LM_SUP_INV_VOTE                -600104
#define LM_SUP_INV_ORDERTYPE           -600106
#define LM_MMSUP_RELOAD_OB              600107
#define LM_MMSUP_NOT_INIT              -600109
#define LM_MMSUP_INVLEN                -600110
#define LM_MMSUP_INVTXTYP              -600112
#define LM_MMSUP_BAD_FORMAT            -600113
#define LM_MMSUP_INV_BOOLEAN           -600115
#define LM_MMSUP_INV_PARAM             -600117
#define LM_MMSUP_INV_TIME              -600119
#define LM_MMSUP_NO_PRICES             -600121
#define LM_MMSUP_NOT_LEGITIMATE        -600123
#define LM_MMSUP_MAR_CLOSED            -600125
#define LM_MMSUP_COMM_STOP             -600127
#define LM_MMSUP_INV_ORDERTYPE         -600129
#define LM_MMSUP_INV_BLOCK             -600131
#define LM_MMSUP_ILL_TRT_IN_USR        -600133
#define LM_MMSUP_TOO_MANY              -600135
#define LM_MMSUP_NONE_FOUND            -600137
#define LM_MMSUP_NOT_USER              -600138
#define LM_LSSRV_MARKET_HOURS          -600140
#define LM_CROSS_REQ_OK                 600141
#define LM_CROSS_REQ_OK_2               600143
#define LM_CROSS_REQ_ENDED              600145
#define LM_CROSS_BLOCK_RELEASED         600147
#define LM_CROSS_MAR_ESTAB              600149
#define LM_CROSS_MAR_TIMOUT             600151
#define LM_CROSS_REQ_MEM_ACT           -600152
#define LM_CROSS_REQ_BLOCKED           -600154
#define LM_CROSS_INSID_NOTFND          -600156
#define LM_CROSS_NOT_USER              -600158
#define LM_CROSS_REQ_NOT_FOUND         -600160
#define LM_CROSS_INT_NOT_ALLOWED       -600162

/******************************************************************************
*end*of* lm_messages.h
*******************************************************************************/

#endif /* _LM_MESSAGES_H_ */

#ifndef _MP_MESSAGES_H_
#define _MP_MESSAGES_H_

/******************************************************************************
Module: mp_messages.h

Message definitions generated 2001-10-25 12:37:20 by amt, version 3.1.0.0.

NOTE: Do not edit this file. It is automatically generated from
      one or more message files.
*******************************************************************************/

#define MP_COUNTERS_NOT_INIT           -405000
#define MP_INFORMATION                  405001
#define MP_SUCCESS                      405003
#define MP_PANIC                       -405004
#define MP_INSFMEM                     -405006
#define MP_BUGCHECK                    -405007
#define MP_QUOTA                       -405009
#define MP_IVBUFLEN                    -405010
#define MP_ABORT                       -405012
#define MP_COMMIT_LOG_UNKNOWN          -405014
#define MP_INVINT                      -405016
#define MP_NOHIT                       -405017
#define MP_LISEMPTY                    -405019
#define MP_OVERFLOW                    -405021
#define MP_RECEXCLEN                   -405022
#define MP_WRNGFTYP                    -405024
#define MP_OPLOG_FULL                  -405026
#define MP_FILE_NOT_FOUND              -405028
#define MP_INFO_ERROR_STATE            -410000
#define MP_INFO_LOG_TOO_SMALL          -410002
#define MP_INFO_NO_TIMESTAMP           -410004
#define MP_INFO_LOG_UNKNOWN            -410006
#define MP_INFO_LOG_WRONG_SIZE         -410008
#define MP_INFO_TOO_MANY_DEALS         -410010
#define MP_INFO_POSS_DUPL              -410011
#define MP_INFO_TOO_MANY_LOGS          -410012
#define MP_INFO_UNDEFINED_STATE        -410014
#define MP_LOG_ALREADY_INIT             415001
#define MP_LOG_NOT_INITIALIZED         -415002
#define MP_LOG_NOT_READ_MODE           -415004
#define MP_LOG_NOT_WRITE_MODE          -415006
#define MP_LOG_END_OF_FILE             -415007
#define MP_LOG_UNDEFINED_MODE          -415008
#define MP_MATCH_ALRLOAD               -420001
#define MP_MATCH_DEFMAX                -420003
#define MP_MATCH_DISCARDED             -420005
#define MP_MATCH_ILL_CRS               -420007
#define MP_MATCH_MIN_QUANTITY          -420009
#define MP_MATCH_PREMIUM_REQ           -420011
#define MP_MATCH_TIMEOUT               -420013
#define MP_MATCH_INV_ALTER             -420015
#define MP_MATCH_NOT_IMPL              -420017
#define MP_MATCH_OVERBID               -420019
#define MP_MATCH_UNKNOWN               -420021
#define MP_MATCH_BID_ASK               -420023
#define MP_MATCH_VALIDITY              -420025
#define MP_MATCH_MARBOUNCE             -420027
#define MP_MATCH_PREMIUM               -420029
#define MP_MATCH_STOPPED               -420031
#define MP_MATCH_AONBOUNCE             -420033
#define MP_MATCH_BLOCK                 -420035
#define MP_MATCH_COMBO_MAX_LEGS        -420037
#define MP_MATCH_COMBO_PART            -420039
#define MP_MATCH_COMBO_SAVE            -420041
#define MP_MATCH_INV_COMBO_SIZE        -420043
#define MP_MATCH_QUANTITY              -420045
#define MP_MATCH_ORDER_EXISTS          -420047
#define MP_MATCH_NOT_AT_MARKET         -420049
#define MP_MATCH_LOW_VOLUME            -420051
#define MP_MATCH_BROKER_SPREAD         -420053
#define MP_MATCH_ILL_INTERBANK         -420055
#define MP_MATCH_ACC_PREMIUM           -420057
#define MP_MATCH_ACC_ORDER             -420059
#define MP_MATCH_COMBO_LEG             -420061
#define MP_MATCH_LEG_PRICE_NOT_0       -420063
#define MP_MATCH_LEG_NOT_FOUND         -420065
#define MP_MATCH_QUANT_NE_LEG          -420067
#define MP_MATCH_COMBO_SPREAD          -420069
#define MP_MATCH_SER_IS_EXPIRED        -420071
#define MP_MATCH_VOLUME_REQ            -420073
#define MP_MATCH_CROSS_NO_STDCOMB      -420074
#define MP_MATCH_ILL_ORDER_TYPE        -420076
#define MP_MATCH_MKT_ORDER_PRICE       -420078
#define MP_MATCH_NOCLS_PRM             -420079
#define MP_MATCH_FILL_BAL              -420081
#define MP_MATCH_COP_LESS              -420083
#define MP_MATCH_COP_VAR               -420085
#define MP_MATCH_PRE_TIME              -420087
#define MP_MATCH_INV_EXP               -420089
#define MP_MATCH_CROSS_PRICE           -420091
#define MP_MATCH_BLOCK_MAX_LEGS        -420093
#define MP_MATCH_BLOCK_SERIES          -420095
#define MP_MATCH_LMT_NOT_VAL           -420097
#define MP_MATCH_MKT_NOT_VAL           -420099
#define MP_MATCH_KIL_NOT_VAL           -420101
#define MP_MATCH_EXISTS_AS_STD_COMBO   -420103
#define MP_MATCH_EXISTS_AS_REV_STD_CBS -420105
#define MP_MATCH_TM_COMBO_EXISTS       -420107
#define MP_MATCH_REV_TM_COMBO_EXISTS   -420109
#define MP_MATCH_TOO_MANY_TM_COMBOS    -420111
#define MP_MATCH_ILL_RATIO             -420113
#define MP_MATCH_ILL_BUY_SELL_OP       -420115
#define MP_MATCH_COMBO_DIFF_CONTR_SIZE -420117
#define MP_MATCH_NO_BB_WHEN_TM_CBS_POS -420119
#define MP_MATCH_DUPL_SERIES           -420121
#define MP_MATCH_NOT_AUTHORIZED        -420123
#define MP_MATCH_NOTHEO_PRM            -420125
#define MP_MATCH_MIXED_COM             -420127
#define MP_MATCH_MEM_CRS_NOT_ALLOWED   -420129
#define MP_MATCH_PRICE_LIMIT           -420131
#define MP_MATCH_INV_STP_COND          -420133
#define MP_MATCH_INV_HIDDEN            -420135
#define MP_MATCH_INV_SHOWN             -420137
#define MP_MATCH_NOT_AUTH              -420139
#define MP_MATCH_SHOWN_TOO_SMALL       -420141
#define MP_MATCH_AONNOTVALID           -420143
#define MP_MATCH_INV_DELTA             -420145
#define MP_MATCH_FOK_NOT_WKUP_OWN      -420147
#define MP_MATCH_AON_CROSS_PRICE       -420149
#define MP_MATCH_LEG_PREMIUM           -420151
#define MP_OB_INV_HANDLE               -425000
#define MP_OB_NO_RESOURCE              -425002
#define MP_OB_NOT_COSTUMER             -425003
#define MP_OB_NOT_INIT                 -425004
#define MP_OB_NOT_LOCKED               -425006
#define MP_OB_NOT_SERIES               -425007
#define MP_OB_OLD_CUSTOMER             -425009
#define MP_OB_OLD_SERIES               -425011
#define MP_OB_END                       425013
#define MP_OB_TENTATIVE                 425015
#define MP_OB_NOT_CUSTOMER             -425017
#define MP_OB_NOT_UPDATER              -425018
#define MP_RTR_ERROR                   -430001
#define MP_RTR_ALRCRE                  -430003
#define MP_RTR_NULLIOB                 -430005
#define MP_RTR_NODST                   -430007
#define MP_QUERY_CUST_IST              -435001
#define MP_QUERY_NOT_OWN               -435003
#define MP_QUERY_CUST_UND              -435005
#define MP_QUERY_OID_NOTFND            -435007
#define MP_QUERY_FAIL                  -435008
#define MP_QUERY_ONLY_THIS_SER         -435009
#define MP_QUEUE_EMPTY                 -440001
#define MP_QUEUE_FULL                  -440003
#define MP_QUEUE_NOT_INIT              -440005
#define MP_QUEUE_SIZE_ERROR            -440006
#define MP_STAT_INSFMEM                -445000
#define MP_STAT_INT_OVRFLW             -445002
#define MP_PBLOG_NO_LOGB_NAME          -450000
#define MP_TXSRV_TIMEOUT               -455001
#define MP_TXSRV_NOT_CONN              -455003
#define MP_MXABS_BAD_ATTRIBUTE         -460000
#define MP_MXABS_OK                     460001
#define MP_MXABS_INVHANDLE             -460003
#define MP_MXABS_INVLEN                -460005
#define MP_MXABS_NOTFND                -460007
#define MP_MXABS_NYI                   -460009
#define MP_MXABS_NOOP4CLIENT           -460011
#define MP_MXABS_NOACCESS              -460013
#define MP_MXABS_NOACTION              -460015
#define MP_MXABS_NOREPORT              -460017
#define MP_MXABS_ALROPEN               -460019
#define MP_MXABS_INVEXERPT             -460021
#define MP_MXABS_SUBJ2DLL              -460023
#define MP_MXABS_INVTXCODE             -460024
#define MP_MXABS_ZEROQTY               -460025
#define MP_MXABS_BADQTY4REM            -460027
#define MP_MXABS_SYNCINPROG            -460029
#define MP_MXABS_DLLINPROG             -460031
#define MP_MXABS_ALRRES                -460033
#define MP_MXABS_NOTRES                -460035
#define MP_MXABS_TRADERSUSP            -460037
#define MP_MXABS_CXLREPLMENT           -460039
#define MP_MXABS_OPMISMATCH            -460041
#define MP_MXABS_ALERT                 -460043
#define MP_MXABS_OK_BUT_WAIT            460045
#define MP_MXAOF_INTERR                -465000
#define MP_MXAOF_COMERR                -465002
#define MP_MXAOF_NOLOG                 -465004
#define MP_MXAOF_MSG_REROUTED          -465005
#define MP_MXAOF_MSG_IGNORED           -465006
#define MP_MXAOF_SEQ_NO_TO_AOF          465007
#define MP_MXAOF_SEQ_NO_FROM_AOF        465009
#define MP_MXFLT_INTERR                -470000
#define MP_MXFLT_COMERR                -470002
#define MP_MXFLT_NOLOG                 -470004
#define MP_MXFLT_DATAERR               -470006
#define MP_MXFLT_HANDLERR              -470008
#define MP_MXFLT_NOTSPEC               -470010
#define MP_MXFLT_NOTFOUND              -470012
#define MP_MXFLT_FILTERR               -470014
#define MP_QXSRV_ILL_MIXED_INV_PRICES  -475001
#define MP_QXSRV_ILL_MIXED_PRICE_TYPES -475003
#define MP_QXSRV_ILL_NO_COMBO_LEGS     -475005
#define MP_QXSRV_NON_EXISTING          -475007
#define MP_QXSRV_NOT_ALLOWED           -475009
#define MP_QXSRV_NOT_ALLOWED_TO_ANSWER -475011
#define MP_QXSRV_NOT_ALLOWED_TO_EXE    -475013
#define MP_QXSRV_QUOTE_MISSING         -475015
#define MP_QXSRV_STATE_NOT_ACCEPTING   -475017
#define MP_QXSRV_STATE_NOT_IN_TRADING  -475019
#define MP_QXSRV_TOO_LONG_ACCEPT_TIMER -475021
#define MP_QXSRV_TOO_LONG_TRADE_TIMER  -475023
#define MP_QXSRV_VOLUME_TOO_SMALL      -475025
#define MP_QXSRV_MAX_NO_COMBO_LEGS     -475027

/******************************************************************************
*end*of* mp_messages.h
*******************************************************************************/

#endif /* _MP_MESSAGES_H_ */
#ifndef _OMN_MESSAGES_H_
#define _OMN_MESSAGES_H_

/******************************************************************************
Module: OMN_MESSAGES.h

Message definitions generated 2001-10-25 12:39:56 by amt, version 3.1.0.0.

NOTE: Do not edit this file. It is automatically generated from
      one or more message files.
*******************************************************************************/

#define OMN_GENERIC_DBG                 200001
#define OMN_GENERIC_SUCCESS             200003
#define OMN_GENERIC_INFO                200005
#define OMN_GENERIC_WARN               -200007
#define OMN_GENERIC_ERROR              -200008
#define OMN_GENERIC_FATAL              -200010
#define OSA_SUCCESS                     201001
#define OSA_PROBLEM                     201003
#define OSA_NEWCCPSRV                   201005
#define OSA_STATE                       201007
#define OSA_INFO                        201009
#define OSA_DUPLICATE                  -201011
#define OSA_BADTABLE                   -201012
#define OSA_NOLOGFILE                  -201014
#define OSA_NOSUCHUSER                 -201016
#define OSA_BADARGVAL                  -201018
#define OSA_INSFMEM                    -201020
#define OSA_MESSED                     -201022
#define OMN_OSA_TXSY_TABLE_FAIL        -202000
#define OMN_OSA_TXSY_RTR_FAIL          -202002
#define OMN_OSA_TXSY_REC_FAIL          -202004
#define OMN_OSA_TBL_NOTRACK             203001
#define OMN_OSA_TBL_BUFF_INTERR        -203002
#define OMN_OSA_TBL_PARAM_INTERR       -203004
#define OMN_OSA_TBL_BUFALMOSTFULL      -203006
#define OMN_OSA_TBL_BUFOVERFLOW        -203008
#define OMN_OSA_TBL_DISTRLIST          -203010
#define OMN_OSA_TBL_INVSNDTBL          -203012
#define OMN_OSA_TBL_INVSNDRTR          -203014
#define OMN_OSA_TBL_INVSECIT           -203016
#define OMN_OSA_SEC_INTERR             -204000
#define OMN_OSA_SEC_OMDU_INVLOGIN      -204002
#define OMN_OSA_SEC_INVLOGIN_1         -204004
#define OMN_OSA_SEC_INVLOGIN_2         -204006
#define OMN_OSA_SEC_INVLOGIN_3         -204007
#define OMN_OSA_SEC_INVLOGIN_4          204009
#define OMN_OSA_SEC_INVLOGIN_5          204011
#define OMN_OSA_SEC_INVLOGIN_6          204013
#define OMN_OSA_SEC_INVLOGIN_7          204015
#define OMN_OSA_SEC_INVLOGIN_8          204017
#define OMN_OSA_SEC_INVLOGIN_9          204019
#define OMN_OSA_SEC_INVLOGIN_10         204021
#define OMN_OSA_SEC_INVLOGIN_11         204023
#define OMN_OSA_SEC_INVLOGIN_12         204025
#define OMN_OSA_SEC_INVLOGIN_13         204027
#define OMN_OSA_SEC_INVLOGIN_15         204029
#define OMN_OSA_SEC_INVLOGIN_16        -204030
#define OMN_OSA_SEC_INVLOGIN_17         204031
#define OMN_OSA_SEC_INVLOGIN_19         204033
#define OMN_OSA_SEC_INVLOGIN_20         204035
#define OMN_OSA_SEC_SET_SRC_FAILED     -204036
#define OMN_OSA_SEC_SET_PASSW_FAILED   -204038
#define OMN_OSA_SEC_INFOMSG             204039
#define OMN_OSA_SEC_SECVIO_1           -204040
#define OMN_OSA_SEC_SECVIO_2           -204042
#define OMN_OSA_SEC_OAPIOLD            -204044
#define OMN_OSA_SEC_INVLOGIN_21        -204046
#define OMN_OSA_SEC_INVNODEID_1        -204048
#define OMN_OSA_SEC_INVNODEID_2        -204050
#define OMN_OSA_SEC_INVNODEID_3        -204052
#define OMN_OSA_SEC_INVNODEID_4        -204054
#define OMN_OSA_SEC_INVNODEID_5        -204056
#define OMN_OSA_SEC_INVNODEID_6        -204058
#define OMN_OSA_SEC_INVNODEID_7        -204060
#define OMN_OSA_SEC_INVNODEID_8        -204062
#define OMN_OSA_SEC_INVNODEID_9        -204064
#define OMN_OSA_SEC_INVNODEID_10       -204066
#define OMN_OSA_SEC_INVNODEID_11       -204068
#define OMN_EVENT_LOG_MESSAGE           205001
#define OMDU_IPMCQUERD                 -205500
#define OMDU_IPMCQUEXTA                -205502
#define OMDU_INVTRNSP                  -205504
#define OMDU_INVUDPPORT                -205506
#define OMDU_OPENSOCK                  -205508
#define OGWY_LOGGEDOUT                  206001
#define OGWY_NORMAL                     206003
#define OGWY_BUFOVRRUN                  206005
#define OGWY_FILENAME                   206007
#define OGWY_DISCONUSER                 206009
#define OGWY_KILLED                     206011
#define OGWY_MISSIMPL                   206013
#define OGWY_NETPROBLEM                 206015
#define OGWY_OPRLOGO                    206017
#define OGWY_REMOTE                     206019
#define OGWY_SHUTSESS                   206021
#define OGWY_TCPSESS                    206023
#define OGWY_TIMEDOUT                   206025
#define OGWY_HBXTAERR                   206027
#define OGWY_VALUES                     206029
#define OGWY_STTOAWAIT                  206031
#define OGWY_STTREQREC                  206033
#define OGWY_STTOMNIPR                  206035
#define OGWY_STTREQRPL                  206037
#define OGWY_STTNOCONN                  206039
#define OGWY_DISCONAPI                 -206041
#define OGWY_INTPROBLEM                -206043
#define OGWY_INVLENGTH                 -206045
#define OGWY_INVNETFNC                 -206047
#define OGWY_NOSUCHFNCN                -206049
#define OGWY_BADAPIREQ                 -206050
#define OGWY_BADDATFIL                 -206052
#define OGWY_ERROPEN                   -206054
#define OGWY_EXECNODE                  -206056
#define OGWY_ILLEGALPORT               -206058
#define OGWY_INTERROR                  -206060
#define OGWY_INVBUFREF                 -206062
#define OGWY_IVDEF                     -206064
#define OGWY_NOCDB                     -206066
#define OGWY_NORECORD                  -206068
#define OGWY_NOLOGNAM                  -206070
#define OGWY_CREEVENT                  -206072
#define DUGWY_NOCFGFILE                -207001
#define DUGWY_INTERR                   -207002
#define DUGWY_NORMTSRVNODES            -207004
#define DUGWY_NORMTSRV                 -207006
#define DUGWY_NOMBRNODES               -207008
#define DUGWY_INFGRPEXISTS             -207010
#define DUGWY_NOINFGRP                 -207012
#define DUGWY_INFGRPASS                -207014
#define DUGWY_NOFILTER                 -207016
#define DUGWY_CFGSTRLNG                -207018
#define DUGWY_NOSRVTCPIP               -207020
#define DUGWY_NOSRVDECNET              -207022
#define DUGWY_NOSRVX25                 -207024
#define DUGWY_NOMEMBER                 -207026
#define DUGWY_INVGWYMSG                -207028
#define DUGWY_INVGWYVRS                -207030
#define DUGWY_NOFLTTYP                 -207032
#define DUGWY_DYNINFCLSEXIST           -207034
#define DUGWY_NODYNINFCLS              -207036
#define DUGWY_INSUFMEM                 -207038
#define DUGWY_TPSNOTSUPP               -207040
#define DUGWY_NOTIME                   -207042
#define DUGWY_ABORT                    -207044
#define DUGWY_NOOPMAP                  -207045
#define DUGWY_EOFFND                    207047
#define DUGWY_CLTQUETOLONG             -207049
#define DUGWY_OVERLOAD                  207051
#define DUGWY_UNKNOWN                   207053
#define DUGWY_CLEANUP                  -207055
#define OMDU_DATALOST                  -208000
#define OMDU_LINKLOST                  -208002
#define OMDU_SENDERLOST                -208004
#define OMDU_NONODADDR                 -208006
#define OMDU_INVFREE                   -208008
#define OMDU_NONODENAME                -208010
#define OMDU_NOCFGFILE                 -208012
#define OMDU_NOCMAADR                  -208014
#define OMDU_NOTMAADR                  -208016
#define OMDU_NORMAADR                  -208018
#define OMDU_INFCLSREQ                 -208020
#define OMDU_INFNOTACT                 -208022
#define OMDU_NOWRTACCESS               -208024
#define OMDU_NORDACCESS                -208026
#define OMDU_NOTTWOCALLBACK            -208028
#define OMDU_INSFMEM                   -208030
#define OMDU_CONNFAIL                  -208032
#define OMDU_NETERROR                  -208034
#define OMDU_INVTHRCNTX                -208036
#define OMDU_DISCFAIL                  -208038
#define OMDU_CLTPRTERR                 -208040
#define OMDU_CLTLNKLOST                -208042
#define OMDU_INVVALSYNTAX              -208044
#define OMDU_TOMUCHDATA                -208046
#define OMDU_CLTNETTHRTERM             -208048
#define OMDU_INVVERSSTR                -208050
#define OMDU_WRGVERS                   -208052
#define OMDU_INFOBJNAMLNG              -208054
#define OMDU_INFOBJNAMWLD              -208056
#define OMDU_SNDNAMLNG                 -208058
#define OMDU_FAILTODCLSRV              -208060
#define OMDU_NOUPDFLG                  -208062
#define OMDU_BLDUPDLST                 -208064
#define OMDU_XCHN                      -208066
#define OMDU_STARTPORT                 -208068
#define OMDU_ENABLEMCA                 -208070
#define OMDU_ETHQUERD                  -208072
#define OMDU_ETHCOMPRD                 -208074
#define OMDU_ETHQUEXTA                 -208076
#define OMDU_ETHCOMPXTA                -208078
#define OMDU_INFCLSNOXTA               -208080
#define OMDU_INFCLSNOTVLD              -208082
#define OMDU_INTERR                    -208084
#define OMDU_ETHRDCHAR                 -208086
#define OMDU_SNDRUNAVL                 -208088
#define OMDU_RMTSNDRLSTCFG             -208090
#define OMDU_RMTSNDRLSTHB              -208092
#define OMDU_RMTSNDRLSTNOTCFG          -208094
#define OMDU_RMTSNDREXIT               -208096
#define OMDU_RETRNSNOTFND              -208098
#define OMDU_SNDRCFG                   -208100
#define OMDU_TOLNGPNDRCVQUE            -208102
#define OMDU_MAXRETRNSRQST             -208104
#define OMDU_LOCCPYFAIL                -208106
#define OMDU_NOTRUNNING                -208108
#define OMDU_RETRNSFREQ                -208110
#define OMDU_THRSTALLED                -208112
#define OMDU_MINSPEED                  -208114
#define OMDU_RMTPRVRRESTRT             -208116
#define OMDU_NORCVRCLT                 -208118
#define OMDU_SCRPTNOTENA               -208120
#define OMDU_MAXINFCLS                 -208122
#define OMDU_NOINFCLS                  -208124
#define OMDU_NOACCESS                  -208126
#define OMDU_CFGSTRLNG                 -208128
#define OMDU_NOCOMMAND                 -208130
#define OMDU_ERRPARSE                  -208132
#define OMDU_NOPRSENTRY                -208134
#define OMDU_INVINFCLS                 -208136
#define OMDU_NOVLDADRSTR               -208138
#define OMDU_NOVLDETHADR               -208140
#define OMDU_INVPRTTYP                 -208142
#define OMDU_NOATTRVAL                 -208144
#define OMDU_TRUNCATTRVAL              -208145
#define OMDU_FLOODED                   -208147
#define OMDU_MAXSUB                    -208149
#define OMDU_NOEVTSHR                  -208151
#define OMDU_TXIDSPLIT                 -208153
#define OMDU_MOREATTRVAL               -208155
#define OMDU_NOMOREMSGS                -208157
#define OMDU_MOREMSGS                  -208159
#define OMDU_NOTFLOODED                -208161
#define OMDU_NOLOGFILE                 -208162
#define OMDU_NOOPMAP                   -208164
#define OMDU_FAILOPNLOGF               -208166
#define OMDU_XTASIZENOLL               -208168
#define OMDU_CLTQUETOLNG               -208170
#define OMDU_NOMAINTSRVFND             -208172
#define OMDU_NOGSTHR                   -208174
#define OMDU_GBLSECUPD                 -208176
#define CACHE_QRYNOCFG                 -209000
#define CACHE_QRYNOAVAIL               -209002
#define CACHE_OVFLOW                   -209004
#define CACHE_COMERR                   -209006
#define CACHE_CFGERR                   -209008
#define CACHE_INTERR                   -209010
#define CACHE_MAXCLI                   -209012
#define CACHE_OKIDOKI                   209013

/******************************************************************************
*end*of* OMN_MESSAGES.h
*******************************************************************************/

#endif /* _OMN_MESSAGES_H_ */
#ifndef _PX_MESSAGES_H_
#define _PX_MESSAGES_H_

/******************************************************************************
Module: px_messages.h

Message definitions generated 2001-04-23 13:20:17 by amt, version V2.4.

NOTE: Do not edit this file. It is automatically generated from
      one or more message files.
*******************************************************************************/

#define HE_DAYAHEAD_DBG                 1800001
#define HE_DAYAHEAD_SUCCESS             1800003
#define HE_DAYAHEAD_INFO                1800005
#define HE_DAYAHEAD_WARN               -1800007
#define HE_DAYAHEAD_ERROR              -1800008
#define HE_DAYAHEAD_FATAL              -1800010
#define HE_DAYAHEAD_INVNUM             -1800012
#define HE_DAYAHEAD_MAXNUM             -1800014
#define HE_DAYAHEAD_INVDATE            -1800016
#define HE_DAYAHEAD_INVTIME            -1800018
#define HE_DAYAHEAD_INVITER            -1800020
#define HE_DAYAHEAD_MINHOUR            -1800022
#define HE_DAYAHEAD_MAXHOUR            -1800024
#define HE_DAYAHEAD_MAXPRICE           -1800026
#define HE_DAYAHEAD_MINPRICE           -1800028
#define HE_DAYAHEAD_INVFORMAT          -1800030
#define HE_DAYAHEAD_INVSENDER          -1800032
#define HE_DAYAHEAD_INVPART            -1800034
#define HE_DAYAHEAD_NOMINPRICE         -1800036
#define HE_DAYAHEAD_NOMAXPRICE         -1800038
#define HE_DAYAHEAD_TWOPRICE           -1800040
#define HE_DAYAHEAD_DECPRICE           -1800042
#define HE_DAYAHEAD_DECQUANT           -1800044
#define HE_DAYAHEAD_INCQUANT           -1800046
#define HE_DAYAHEAD_MAXQUANT           -1800048
#define HE_DAYAHEAD_MAXRAMP            -1800050
#define HE_DAYAHEAD_NUMRESOURCE        -1800052
#define HE_DAYAHEAD_NEGNUMBER          -1800054
#define HE_DAYAHEAD_MORETHAN11         -1800056
#define HE_DAYAHEAD_INVPORTFOLIO       -1800058
#define HE_DAYAHEAD_INVRESOURCE        -1800060
#define HE_DAYAHEAD_INVPRIORITYTYPE    -1800062
#define HE_DAYAHEAD_MINQUANT           -1800064
#define HE_DAYAHEAD_ANCSERVNOTALLOWED  -1800066
#define HE_DAYAHEAD_NOSUPPLENERGY      -1800068
#define HE_DAYAHEAD_ZEROBIDSNOTALLOWED -1800070
#define HE_DAYAHEAD_NOVALIDBID         -1800072
#define HE_DAYAHEAD_INCBREAKPOINTS     -1800074
#define HE_DAYAHEAD_DECBREAKPOINTS     -1800076
#define HE_DAYAHEAD_QUANTITYCHANGED    -1800078
#define HE_DAYAHEAD_PRICEINCITERATION  -1800080
#define HE_DAYAHEAD_PRICEDECITERATION  -1800082
#define HE_DAYAHEAD_INVALIDACTIVECHNG  -1800084
#define HE_DAYAHEAD_INVALIDFROZENCHNG  -1800086
#define HE_DAYAHEAD_PRICESNOTEQUAL     -1800088
#define HE_DAYAHEAD_PRICEGTMCP         -1800090
#define HE_DAYAHEAD_PRICENOTEQMCP      -1800092
#define HE_DAYAHEAD_PRICELTMCP         -1800094
#define HE_DAYAHEAD_LASTPRICELTMCP     -1800096
#define HE_DAYAHEAD_LASTQUANTLTIPS     -1800098
#define HE_DAYAHEAD_FIRSTQUANTGTIPS    -1800100
#define HE_DAYAHEAD_FIRSTPRICEGTMCP    -1800102
#define HE_DAYAHEAD_FIRSTQUANTLTIPS    -1800104
#define HE_DAYAHEAD_LASTQUANTGTIPS     -1800106
#define HE_DAYAHEAD_MAXPRICEEXCEEDED   -1800108
#define HE_DAYAHEAD_MINPRICEEXCEEDED   -1800110
#define HE_DAYAHEAD_POSREGRANGEDOWN    -1800112
#define HE_DAYAHEAD_INVSYNCTIME        -1800114
#define HE_DAYAHEAD_CAPGTIPS           -1800116
#define HE_DAYAHEAD_INVCURTAILMENT     -1800118
#define HE_DAYAHEAD_QUANTSNOTEQUAL     -1800120
#define HE_DAYAHEAD_FIRSTQUANTNOTZERO  -1800122
#define HE_DAYAHEAD_QUANTLTCAP         -1800124
#define HE_DAYAHEAD_QUANTGTREGRANGEDOW -1800126
#define HE_DAYAHEAD_MAXOPERLIMIT       -1800128
#define HE_DAYAHEAD_MINOPERLIMIT       -1800130
#define HE_DAYAHEAD_RAMPRATEEXCEEDED   -1800132
#define HE_DAYAHEAD_CAPGTMAXOPERLIMIT  -1800134
#define HE_DAYAHEAD_CAPLTMINOPERLIMIT  -1800136
#define HE_DAYAHEAD_HOLEINBID          -1800138
#define HE_DAYAHEAD_NOQUANTSUBRES      -1800140
#define HE_DAYAHEAD_LASTPRICEGTMCP     -1800142
#define HE_DAYAHEAD_FIRSTPRICELTMCP    -1800144
#define HE_DAYAHEAD_MINPRICENOTREACHED -1800146
#define HE_DAYAHEAD_MAXPRICENOTREACHED -1800148
#define HE_DAYAHEAD_CAPGTMINOPERLIMIT  -1800150
#define HE_DAYAHEAD_MFUPDATENEEDED     -1800152
#define HE_DAYAHEAD_ONLYONECONTTYPE    -1800154
#define HE_DAYAHEAD_INVCONTIDRELMR     -1800156
#define HE_DAYAHEAD_TOOMANYETCID       -1800158
#define HE_DAYAHEAD_HOLEINETCID        -1800160
#define HE_DAYAHEAD_INVCONTIDETC       -1800162
#define HE_DAYAHEAD_ETCFIRSTSTARTZONE  -1800164
#define HE_DAYAHEAD_ETCLASTENDZONE     -1800166
#define HE_DAYAHEAD_MORETHANONECONTBP  -1800168
#define HE_DAYAHEAD_NOCONTRACTID       -1800170
#define HE_DAYAHEAD_NOCONTRACTSCHED    -1800172
#define HE_DAYAHEAD_INVCONTPARAMS      -1800174
#define HE_DAYAHEAD_FSTCONTQNTZERO     -1800176
#define HE_DAYAHEAD_LSTCONTQNTZERO     -1800178
#define HE_DAYAHEAD_LSTCONTQGTSCHED    -1800180
#define HE_DAYAHEAD_FSTCONTQGTSCHED    -1800182
#define HE_DAYAHEAD_ADJMINPRICE        -1800184
#define HE_DAYAHEAD_ADJMAXPRICE        -1800186
#define HE_DAYAHEAD_ASENMINPRICE       -1800188
#define HE_DAYAHEAD_ASENMAXPRICE       -1800190
#define HE_DAYAHEAD_ASCAPMINPRICE      -1800192
#define HE_DAYAHEAD_ASCAPMAXPRICE      -1800194
#define HE_DAYAHEAD_TAVERSIONERROR     -1800196
#define HE_DAYAHEAD_TAVERSIONTOONEW    -1800198
#define HE_DAYAHEAD_TAVERSIONTOOOLD    -1800200
#define HE_DAYAHEAD_PORTNOTENGMRKT     -1800202
#define HE_DAYAHEAD_MINQLTZERO         -1800204
#define HE_DAYAHEAD_QLTMINQ            -1800206
#define HE_DAYAHEAD_QGTMAXQ            -1800208
#define HE_DAYAHEAD_CAPGTRAMP30        -1800210
#define HE_DAYAHEAD_ABSCAPGTRAMP30     -1800212
#define HE_DAYAHEAD_LASTQNOTZERO       -1800214
#define HE_DAYAHEAD_NONDECPRICE        -1800216
#define HE_HOURAHEAD_DBG                1801001
#define HE_HOURAHEAD_SUCCESS            1801003
#define HE_HOURAHEAD_INFO               1801005
#define HE_HOURAHEAD_WARN              -1801007
#define HE_HOURAHEAD_ERROR             -1801008
#define HE_HOURAHEAD_FATAL             -1801010
#define HE_HOURAHEAD_INVNUM            -1801012
#define HE_HOURAHEAD_MAXNUM            -1801014
#define HE_HOURAHEAD_INVDATE           -1801016
#define HE_HOURAHEAD_INVTIME           -1801018
#define HE_HOURAHEAD_INVITER           -1801020
#define HE_HOURAHEAD_MINHOUR           -1801022
#define HE_HOURAHEAD_MAXHOUR           -1801024
#define HE_HOURAHEAD_MAXPRICE          -1801026
#define HE_HOURAHEAD_MINPRICE          -1801028
#define HE_HOURAHEAD_INVFORMAT         -1801030
#define HE_HOURAHEAD_INVSENDER         -1801032
#define HE_HOURAHEAD_INVPART           -1801034
#define HE_HOURAHEAD_NOMINPRICE        -1801036
#define HE_HOURAHEAD_NOMAXPRICE        -1801038
#define HE_HOURAHEAD_TWOPRICE          -1801040
#define HE_HOURAHEAD_DECPRICE          -1801042
#define HE_HOURAHEAD_DECQUANT          -1801044
#define HE_HOURAHEAD_INCQUANT          -1801046
#define HE_HOURAHEAD_MAXQUANT          -1801048
#define HE_HOURAHEAD_MAXRAMP           -1801050
#define HE_HOURAHEAD_NUMRESOURCE       -1801052
#define HE_HOURAHEAD_NEGNUMBER         -1801054
#define HE_HOURAHEAD_MORETHAN11        -1801056
#define HE_HOURAHEAD_INVPORTFOLIO      -1801058
#define HE_HOURAHEAD_INVRESOURCE       -1801060
#define HE_HOURAHEAD_INVPRIORITYTYPE   -1801062
#define HE_HOURAHEAD_MINQUANT          -1801064
#define HE_HOURAHEAD_ANCSERVNOTALLOWED -1801066
#define HE_HOURAHEAD_NOSUPPLENERGY     -1801068
#define HE_HOURAHEAD_INVHOUR           -1801070
#define HE_HOURAHEAD_INVRAMPRATE       -1801072
#define HE_HOURAHEAD_INVCONTRACTMW     -1801074
#define HE_HOURAHEAD_INVCONTRACTREF    -1801076
#define HE_HOURAHEAD_INVENERGYTYPE     -1801078
#define HE_HOURAHEAD_INVQUANT          -1801080
#define HE_HOURAHEAD_PRICENOTFOUND     -1801082
#define HE_HOURAHEAD_NODASCHED         -1801084
#define HE_HOURAHEAD_CAPGTSCHED        -1801086
#define HE_HOURAHEAD_PRICESNOTEQUAL    -1801088
#define HE_HOURAHEAD_PRICEGTMCP        -1801090
#define HE_HOURAHEAD_PRICENOTEQMCP     -1801092
#define HE_HOURAHEAD_PRICELTMCP        -1801094
#define HE_HOURAHEAD_LASTPRICELTMCP    -1801096
#define HE_HOURAHEAD_LASTQUANTLTIPS    -1801098
#define HE_HOURAHEAD_FIRSTQUANTGTIPS   -1801100
#define HE_HOURAHEAD_FIRSTPRICEGTMCP   -1801102
#define HE_HOURAHEAD_FIRSTQUANTLTIPS   -1801104
#define HE_HOURAHEAD_LASTQUANTGTIPS    -1801106
#define HE_HOURAHEAD_MAXPRICEEXCEEDED  -1801108
#define HE_HOURAHEAD_MINPRICEEXCEEDED  -1801110
#define HE_HOURAHEAD_POSREGRANGEDOWN   -1801112
#define HE_HOURAHEAD_INVSYNCTIME       -1801114
#define HE_HOURAHEAD_INVCURTAILMENT    -1801116
#define HE_HOURAHEAD_QUANTSNOTEQUAL    -1801118
#define HE_HOURAHEAD_FIRSTQUANTNOTZERO -1801120
#define HE_HOURAHEAD_QUANTLTCAP        -1801122
#define HE_HOURAHEAD_QUANTGTREGRANGEDO -1801124
#define HE_HOURAHEAD_MAXOPERLIMIT      -1801126
#define HE_HOURAHEAD_MINOPERLIMIT      -1801128
#define HE_HOURAHEAD_RAMPRATEEXCEEDED  -1801130
#define HE_HOURAHEAD_CAPGTMAXOPERLIMIT -1801132
#define HE_HOURAHEAD_CAPLTMINOPERLIMIT -1801134
#define HE_HOURAHEAD_HOLEINBID         -1801136
#define HE_HOURAHEAD_NOQUANTSUBRES     -1801138
#define HE_HOURAHEAD_LASTPRICEGTMCP    -1801140
#define HE_HOURAHEAD_FIRSTPRICELTMCP   -1801142
#define HE_HOURAHEAD_MINPRICENOTREACHE -1801144
#define HE_HOURAHEAD_MAXPRICENOTREACHE -1801146
#define HE_HOURAHEAD_CAPGTMINOPERLIMIT -1801148
#define HE_HOURAHEAD_ONLYONECONTTYPE   -1801150
#define HE_HOURAHEAD_INVCONTIDRELMR    -1801152
#define HE_HOURAHEAD_TOOMANYETCID      -1801154
#define HE_HOURAHEAD_HOLEINETCIDE      -1801156
#define HE_HOURAHEAD_INVCONTIDETC      -1801158
#define HE_HOURAHEAD_ETCFIRSTSTARTZONE -1801160
#define HE_HOURAHEAD_ETCLASTENDZONE    -1801162
#define HE_HOURAHEAD_MORETHANONECONTBP -1801164
#define HE_HOURAHEAD_NOCONTRACTID      -1801166
#define HE_HOURAHEAD_NOCONTRACTSCHED   -1801168
#define HE_HOURAHEAD_INVCONTPARAMS     -1801170
#define HE_HOURAHEAD_FSTCONTQNTZERO    -1801172
#define HE_HOURAHEAD_LSTCONTQNTZERO    -1801174
#define HE_HOURAHEAD_LSTCONTQGTSCHED   -1801176
#define HE_HOURAHEAD_FSTCONTQGTSCHED   -1801178
#define HE_HOURAHEAD_ADJMINPRICE       -1801180
#define HE_HOURAHEAD_ADJMAXPRICE       -1801182
#define HE_HOURAHEAD_ASENMINPRICE      -1801184
#define HE_HOURAHEAD_ASENMAXPRICE      -1801186
#define HE_HOURAHEAD_ASCAPMINPRICE     -1801188
#define HE_HOURAHEAD_ASCAPMAXPRICE     -1801190
#define HE_HOURAHEAD_SEMINPRICE        -1801192
#define HE_HOURAHEAD_SEMAXPRICE        -1801194
#define HE_HOURAHEAD_TAVERSIONERROR    -1801196
#define HE_HOURAHEAD_TAVERSIONTOONEW   -1801198
#define HE_HOURAHEAD_TAVERSIONTOOOLD   -1801200
#define HE_HOURAHEAD_RESNOTENGMRKT     -1801202
#define HE_HOURAHEAD_MINQLTZERO        -1801204
#define HE_HOURAHEAD_QLTMINQ           -1801206
#define HE_HOURAHEAD_QGTMAXQ           -1801208
#define HE_HOURAHEAD_MINQGTZERO        -1801210
#define HE_HOURAHEAD_QGTMINQ           -1801212
#define HE_HOURAHEAD_QLTMAXQ           -1801214
#define HE_HOURAHEAD_SCHEDWBLTZERO     -1801216
#define HE_HOURAHEAD_SCHEDWDBGTMAXCAP  -1801218
#define HE_HOURAHEAD_MINOPGTEQMAXOP    -1801220
#define HE_HOURAHEAD_CAPGTRAMP30       -1801222
#define HE_HOURAHEAD_ABSCAPGTRAMP30    -1801224
#define HE_HOURAHEAD_LASTQNOTZERO      -1801226
#define HE_HOURAHEAD_NONDECPRICE       -1801228
#define COM_DBG                         1802001
#define COM_NOEVENT                     1802003
#define COM_INIFAIL                    -1802004
#define COM_DCLFAIL                    -1802006
#define COM_NOMEM                      -1802008
#define COM_CLOSEFAIL                  -1802010
#define COM_SHUTFAIL                   -1802012
#define COM_CONNFAIL                   -1802014
#define COM_SNDFAIL                    -1802016
#define COM_RDFAIL                     -1802018
#define COM_NOHDL                      -1802020
#define COM_ERRCHKSUM                  -1802022
#define COM_TRUNCATE                   -1802024
#define COM_TOMANYHDL                  -1802026
#define COM_WRONGREASON                -1802028
#define COM_BADLENGTH                  -1802030
#define COM_BADMESSAGE                 -1802032
#define COM_BADVERSION                 -1802034
#define COM_BADSEQNBR                  -1802036
#define COM_OLDSEQNBR                  -1802038
#define COM_INVSEQNBR                  -1802040
#define COM_RSTRTABORT                 -1802042
#define COM_RSTRTTIMOUT                -1802044
#define COM_MSGTOOLARGE                -1802046
#define COM_FATAL                      -1802048
#define COM_OPERDISC                    1802049

/******************************************************************************
*end*of* px_messages.h
*******************************************************************************/

#endif /* _PX_MESSAGES_H_ */

#ifndef _RI_MESSAGES_H_
#define _RI_MESSAGES_H_

/******************************************************************************
Module: ri_messages.h

Message definitions generated 2001-08-15 17:01:25 by amt, version V2.4.

NOTE: Do not edit this file. It is automatically generated from
      one or more message files.
*******************************************************************************/

#define RI_NORMAL                       900001
#define RI_NYI                         -900002
#define RI_BADMEMBER                   -900004
#define RI_PANIC                       -900006
#define RI_BADSIZE                     -900008
#define RI_BADFORMAT                   -900010
#define RI_TOOMUCHDATA                 -900012
#define RI_ITEXC                       -900014
#define RI_INVPAR                      -900016
#define RI_RANGERR                     -900018
#define RI_NOMEM                       -900020
#define RI_INVPTR                      -900022
#define RI_INVSUB                      -900024
#define RI_ACCVIO                      -900026
#define RI_INTOFLOW                    -900028
#define RI_FLOOFLOW                    -900030
#define RI_FLOUFLOW                     900031
#define RI_INTDIVZERO                  -900032
#define RI_FLODIVZERO                  -900034
#define RI_EOF                         -900035
#define RI_NOTFOUND                    -900037
#define RI_NOROUNDING                  -900039
#define RI_EXTERR                      -900040
#define RI_EXTWARN                     -900041
#define RI_EXTFATAL                    -900042
#define RI_INVTXSTATE                  -900044
#define RI_INVCMD                      -900045
#define RI_UNSUPPCMD                   -900047
#define RI_BUFTOSMALL                  -900048
#define RI_HPARITHTRAP                 -900050
#define RI_REPLAY                       900051
#define RI_INVEXCHANGE                 -900052
#define RI_WAIT_CDB                     900053
#define RI_WAIT_IN_NPSRV                900055
#define RI_WAIT_IN_SRV                  900057
#define RI_WAIT_CL_DELIV                900059
#define RI_WAIT_CL_FIX                  900061
#define RI_WAIT_CL_POS                  900063
#define RI_WAIT_CL_REV_TRADE            900065
#define RI_WAIT_CL_TRADE                900067
#define RI_WAIT_RTR                     900069
#define RI_WAIT_OMDU                    900071
#define RI_ILLEGAL_NODE                -900072
#define RI_INVINT                      -900074
#define RI_NTNOMORE                    -900076
#define RI_NTINVTAB                    -900078
#define RI_NTDUPNAM                    -900080
#define RI_NTNOROOM                    -900082
#define RI_NTNOTFOUND                  -900084
#define RI_NTEXHASH                    -900086
#define RI_NTNOIX                      -900088
#define RI_POSFULL                     -900090
#define RI_POSNOMORE                   -900092
#define RI_POSNOPOINT                  -900094
#define RI_POSUFULL                    -900096
#define RI_TINOVAL                     -900098
#define RI_TIINVTIM                    -900100
#define RI_TIEXPTABFULL                -900102
#define RI_TIVALALEX                   -900103
#define RI_TIIMMTABFULL                -900104
#define RI_HLDINVTYP                   -900106
#define RI_HLDAGREQ                    -900108
#define RI_HLDAGALEX                   -900110
#define RI_HLDAGNOTVALID               -900112
#define RI_HLDNOMEMB                   -900113
#define RI_HLDNOMORE                   -900115
#define RI_HLDPOSALEX                  -900116
#define RI_HLDNOPOS                    -900117
#define RI_HLDNOMAR                    -900119
#define RI_HLDNOMEMMAR                 -900121
#define RI_HLDMARALEX                  -900123
#define RI_HLDNOPRI                    -900125
#define RI_HLDNOMEMPRI                 -900127
#define RI_HLDPRIALEX                  -900129
#define RI_HLDSVALEX                   -900131
#define RI_HLDNOSV                     -900133
#define RI_VALNOVAL                    -900134
#define RI_VALSWNOPER                  -900136
#define RI_REPFMTOFLOW                 -900138
#define RI_MRINVMARK                   -900140
#define RI_MRINVGROUP                  -900142
#define RI_MRINVPARAM                  -900144
#define RI_MRNOPARAM                   -900146
#define RI_MRNOCTRLDEF                 -900148
#define RI_MRNOCORCLA                  -900150
#define RI_MRNOCORSUP                  -900151
#define RI_PRINVMARK                   -900152
#define RI_PRINVGROUP                  -900154
#define RI_PRINVPARAM                  -900156
#define RI_PRNOSYNTHFUT                -900158
#define RI_PRIINVGROUP                 -900160
#define RI_CCTNONODE                   -900162
#define RI_CCTNOMORENOD                -900164
#define RI_CCTNOMOREUNI                -900166
#define RI_CCTTOOMANYPOS               -900168
#define RI_CHFULL                      -900170
#define RI_TDMARFULL                   -900172
#define RI_TDMARALEX                   -900174
#define RI_TDNTDFULL                   -900176
#define RI_TDNTDALEX                   -900178
#define RI_CCTILLEGALMIX               -900180
#define RI_TNMISMATCH                  -900182
#define RI_TNITEXC                     -900184
#define RI_TNILLEGALMIX                -900186
#define RI_TNNODATE                    -900188
#define RI_NOFIX                       -900190
#define RI_QRYUNDEFINED                 900191
#define RI_QRYUNKNOWN                   900193
#define RI_MARG_LIMIT_EXCEEDED         -900195
#define RI_PRICE_LIMIT_EXCEEDED        -900197
#define RI_INVALID_CONFIG_VALUE        -900198
#define RI_HLDFULL                     -900200
#define RI_MRFULL                      -900202
#define RI_MRCORRFULL                  -900204
#define RI_PRFULL                      -900206
#define RI_VALFULL                     -900208
#define RI_HLDNOAG                     -900210
#define RI_CCTFORBIDDENMIX             -900212
#define RI_NTSIZEDIFF                  -900214
#define RI_MRTIERFULL                  -900216
#define RI_MRIMFULL                    -900218
#define RI_MRICFULL                    -900220
#define RI_INVRISKEXP                  -900222
#define RI_INVWROPTMIN                 -900224
#define RI_PRINVSPR                    -900226
#define RI_INVLEG                      -900228
#define RI_PRTICKFULL                  -900230
#define RI_PRSPREADFULL                -900232
#define RI_INVRATIO                    -900234
#define RI_DHNOMOREPOSNOD              -900236
#define RI_DHNOMORERPFNOD              -900238
#define RI_OMN_NORMAL                   910001
#define RI_OMN_BATNTRN                  910003
#define RI_OMN_NOINFO                   910005
#define RI_OMN_UKNWNQRY                -910006
#define RI_OMN_BADSIZE                 -910008
#define RI_OMN_SNDFAIL                 -910010
#define RI_OMN_BADSEG                  -910012
#define RI_OMN_BADEXCHANGE             -910014
#define RI_OMN_BADMARKET               -910016
#define RI_OMN_NOSUP                   -910018
#define RI_OMN_BADDATE                 -910020
#define RI_OMN_BADSIMVALUE             -910022
#define RI_OMN_BADPRICESIM             -910024
#define RI_OMN_BADVOLVALUE             -910026
#define RI_OMN_BADOLEVEL               -910028
#define RI_OMN_BADSEGMENT              -910030
#define RI_OMN_BADSERIES               -910032
#define RI_OMN_MRPARAM                 -910034
#define RI_OMN_PRPARAM                 -910036
#define RI_OMN_VALINT                  -910038
#define RI_OMN_MRCNTRL                 -910040
#define RI_OMN_GETMTRX                 -910042
#define RI_OMN_MRGMTRX                 -910044
#define RI_OMN_VALFAILED               -910046
#define RI_OMN_SIMDENY                 -910048
#define RI_OMN_SIMNEG                  -910050
#define RI_OMN_SIMNOMTCH               -910052
#define RI_OMN_BADTYPE                 -910054
#define RI_OMN_INVALIDDATES            -910056
#define RI_OMN_INVALIDFLAGS            -910058

/******************************************************************************
*end*of* ri_messages.h
*******************************************************************************/

#endif /* _RI_MESSAGES_H_ */
#ifndef _SE_MESSAGES_H_
#define _SE_MESSAGES_H_

/******************************************************************************
Module: se_messages.h

Message definitions generated 2001-06-07 14:49:42 by amt, version V2.4.

NOTE: Do not edit this file. It is automatically generated from
      one or more message files.
*******************************************************************************/

#define SE_OK                           1000001
#define SE_COMPLETED                    1000003
#define SE_NYI                         -1000005
#define SE_PANIC                       -1000006
#define SE_UKNWNTXN                    -1000008
#define SE_PRTCLERR                    -1000010
#define SE_FAILURE                     -1000012
#define SE_INSUFFARG                   -1000014
#define SE_BADARG                      -1000016
#define SE_OMN_NORMAL                   1004001
#define SE_OMN_BATNTRN                  1004003
#define SE_OMN_NOINFO                   1004005
#define SE_OMN_UNAUTHORIZED             1004007
#define SE_OMN_UKNWNQRY                -1004008
#define SE_OMN_BADSIZE                 -1004010
#define SE_OMN_BADSEG                  -1004012
#define SE_OMN_DB_LOCKED               -1004014
#define SE_OMN_INTERR                  -1004016

/******************************************************************************
*end*of* se_messages.h
*******************************************************************************/

#endif /* _SE_MESSAGES_H_ */

#ifndef _SU_MESSAGES_H_
#define _SU_MESSAGES_H_

/******************************************************************************
Module: su_messages.h

Message definitions generated 2001-10-01 14:42:26 by amt, version V2.4.

NOTE: Do not edit this file. It is automatically generated from
      one or more message files.
*******************************************************************************/

#define SU_NORMAL                       1100001
#define SU_TXALRDHDL                    1100003
#define SU_INVTRANSSIZE                -1100004
#define SU_INVTRANSTYPE                -1100006
#define SU_INV_BLOCK                   -1100007
#define SU_INV_QUANTITY                -1100009
#define SU_INV_PREMIUM                 -1100011
#define SU_INV_OPEN_CLOSE_REQ          -1100013
#define SU_INTERNALERROR               -1100015
#define SU_INVUSER                     -1100017
#define SU_INVTX                       -1100019
#define SU_INVSEGMENTNR                -1100021
#define SU_NOTOPEN                     -1100023
#define SU_FILEOPRFAILED               -1100025
#define SU_INSFMEM                     -1100026
#define SU_BUGCHECK                    -1100028
#define SU_ABORT                       -1100030
#define SU_MATCH_VALIDITY              -1100031
#define SU_PARAMETER_FILEOPRFAILED     -1100032
#define SU_NOT_ACTIVE_BIN              -1100033
#define SU_INVPARAMETER                -1100035
#define SU_INVINSTRCLASS               -1100037
#define SU_INVMM                       -1100039
#define SU_INVPMM                      -1100041
#define SU_NON_CUST_TRADE_LIM_ABS_LOW  -1100043
#define SU_NON_CUST_TRADE_LIM_FRAC_LOW -1100045
#define SU_NON_CUST_TRADE_LIM_FRAC_HIG -1100047
#define SU_FARMM_TRADE_LIM_FRAC_HIGH   -1100049
#define SU_FARMM_TRADE_LIM_FRAC_LOW    -1100051
#define SU_TICK_WORSE_VOLUME_LOW       -1100053
#define SU_DERIVED_ORDER_MAX_SIZE_HIGH -1100055
#define SU_INV_EXPIRATION_GROUP        -1100057
#define SU_INV_STRIKE_PRICE_GROUP      -1100059
#define SU_TIMENOTFUTURE               -1100061
#define SU_ITEMEXIST                   -1100063
#define SU_NOITEMEXIST                 -1100065
#define SU_SERIES_STOPPED_CRS          -1100067
#define SU_COMALRACT                   -1100069
#define SU_COMALRSUS                   -1100071
#define SU_INV_TRADING_STATE           -1100073
#define SU_NO_NEXT_STATE               -1100075
#define SU_INV_DESIGNATED_LIST         -1100077
#define SU_INV_CONCERN_VALUE           -1100079
#define SU_INV_CLASS_FAST_MARKET       -1100081
#define SU_INV_FAST_MARKET_LEVEL       -1100083
#define SU_INVUNDERLYING               -1100085
#define SU_EXPO_LIMIT_FILEOPRFAILED    -1100086
#define SU_CURTAIL_SPR_FACT_LOW        -1100087
#define SU_CURTAIL_SPR_FACT_HIG        -1100089

/******************************************************************************
*end*of* su_messages.h
*******************************************************************************/

#endif /* _SU_MESSAGES_H_ */
