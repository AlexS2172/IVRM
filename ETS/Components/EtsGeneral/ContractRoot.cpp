#include "StdAfx.h"
#include "ContractRoot.h"

//---------------------------------------------------------//
CContractRoot::CContractRoot(void):
m_ID(BAD_LONG_VALUE),
m_Symbol(""),
m_Name(""),
m_QuotationUnitName(""),
m_LotSize(1),
m_TickSize(1.),
m_TickValue(1.),
m_PriceQuotationUnit(1.),
m_QuotationUnitID(BAD_LONG_VALUE),
m_QuoteFormat(BAD_LONG_VALUE),
m_AssetID(BAD_LONG_VALUE){
};
//---------------------------------------------------------//
CContractRoot::~CContractRoot(void){
};
//---------------------------------------------------------//
//long
//CContractRoot::getID(){
//	return m_ID;
//};
////---------------------------------------------------------//
//void
//CContractRoot::setID(long ID){
//	m_ID = ID;
//};
////---------------------------------------------------------//
//string
//CContractRoot::getName(){
//	return m_Name;
//};
////---------------------------------------------------------//
//void
//CContractRoot::setName(string Name){
//	m_Name = Name;
//};
////---------------------------------------------------------//
//string
//CContractRoot::getSymbol(){
//	return m_Symbol;
//};
////---------------------------------------------------------//
//void
//CContractRoot::setSymbol(string Symbol){
//	m_Symbol = Symbol;
//};
////---------------------------------------------------------//
//ExerciseStyleEnum
//CContractRoot::getExerciseStyle(){
//	return m_ExerciseStyle;
//};
////---------------------------------------------------------//
//void
//CContractRoot::setExrciseStyle(ExerciseStyleEnum Style){
//	m_ExerciseStyle = Style;
//};
////---------------------------------------------------------//
//DATE
//CContractRoot::getSettlementValueTime(){
//	return m_SettlementValueTime;
//};
////---------------------------------------------------------//
//void
//CContractRoot::setSettlementValueTime(DATE ValueTime){
//	m_SettlementValueTime = ValueTime;
//};
//---------------------------------------------------------//
//---------------------------------------------------------//
