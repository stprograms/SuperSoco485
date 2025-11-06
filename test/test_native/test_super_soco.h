#ifndef TEST_SUPER_SOCO_H
#define TEST_SUPER_SOCO_H

void test_super_soco_init_no_param(void);
void test_super_soco_init_with_param(void);
void test_super_soco_parse_0_chunk(void);
void test_super_soco_data_updated(void);
void test_super_soco_battery_data(void);
void test_super_soco_ecu_data(void);

void test_telegram_parser_initial_state();
void test_telegram_parser_stored_data(void);
void test_telegram_parser_flush(void);
void test_telegram_parser_incomplete_no_callback(void);
void test_telegram_parser_incomplete_and_new_no_callback(void);
void test_telegram_parser_complete_telegram_callback(void);
void test_telegram_parser_incomplete_dropped();
void test_telegram_parser_states(void);

#endif // TEST_SUPER_SOCO_H
