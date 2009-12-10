/*
 * CCollection.cpp
 *
 *  Created on: 17 Oct 2009
 *      Author: martin
 */


//CCollection::CCollection() {
//	// TODO Auto-generated constructor stub
//
//}
//
//CCollection::~CCollection() {
//	// TODO Auto-generated destructor stub
//}
//
//int CCollection::size()
//{
//	return m_items.size();
//}
//
//void CCollection::append(QList<CCollectionItem> newItems)
//{
//	m_items.append(newItems);
//}
//
//void CCollection::append(CCollectionItem newItem)
//{
//	m_items.append(newItem);
//}
//
//QString CCollection::data(int row, int column)
//{
//	QString data;
//
//	if(row > size() || column > 5)
//	{
//		return QString();
//	}
//
//	const CCollectionItem &item = m_items.at(row);
//	switch(column)
//	{
//		case 0:
//			data = item.getArtist();
//			break;
//		case 1:
//			data = item.getAlbum();
//			break;
//
//		case 2:
//			data = QString().number(item.getYear());
//			break;
//
//		case 3:
//			data = item.getTitle();
//			break;
//
//		case 4:
//		{
//			int secs = item.getLengthInSec();
//			data = QString("%1:%2").arg(secs / 60).arg(secs % 60);
//			break;
//		}
//		default:
//			return QString("Hä?");
//	}
//	return data;
//
//}
//
//QString CCollection::getTitle(int col)
//{
//	QString title;
//	switch(col)
//	{
//		case 0:
//			title = "Artist";
//			break;
//		case 1:
//			title = "Album";
//			break;
//
//		case 2:
//			title = "Year";
//			break;
//
//		case 3:
//			title = "Title";
//			break;
//
//		case 4:
//			title = "Length";
//			break;
//
//		default:
//			return QString("Hä?");
//
//	}
//	return title;
//}
//
//QString CCollection::asString()
//{
//	QString text;
//	for(int i = 0; i < m_items.size(); i++)
//	{
//		text.append(m_items.at(i).asString());
//		text.append("\n");
//	}
//	return text;
//}
//
//
//QString CCollection::getItemAsString(int pos)
//{
//	return m_items.at(pos).asString();
//}
//
//void CCollection::insertItem(CCollectionItem item, int pos)
//{
//	m_items.insert(pos, item);
//}
//
//void CCollection::insertItems(QList<CCollectionItem> items, int pos)
//{
//	// TODO clever handling of multiple inserts
//	for(int i = 0; i < items.size(); i++)
//	{
//		m_items.insert(pos + i, items.at(i));
//	}
//}
//
//void CCollection::removeItem(int pos)
//{
//	m_items.removeAt(pos);
//}
//
//void CCollection::removeItems(int pos, int num)
//{
//	// TODO clever handling of multiple removes
//	for(int i = 0; i < num; i++)
//	{
//		m_items.removeAt(pos + i);
//	}
//}
