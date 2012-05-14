/**
 * @file DataContainer.h
 * @brief 
 *
 * This file is created at Almende B.V. It is open-source software and part of the Common 
 * Hybrid Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from 
 * thread pools and TCP/IP components to control architectures and learning algorithms. 
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless,
 * we personally strongly object against this software used by the military, in the
 * bio-industry, for animal experimentation, or anything that violates the Universal
 * Declaration of Human Rights.
 *
 * Copyright © 2010 Anne van Rossum <anne@almende.com>
 *
 * @author 	Anne C. van Rossum
 * @date	Jul 21, 2011
 * @project	Replicator FP7
 * @company	Almende B.V.
 * @case	Self-organised criticality
 */


#ifndef DATADECORATOR_H_
#define DATADECORATOR_H_

// General files
#include <map>
#include <math.h>

/* **************************************************************************************
 * Interface of DataContainer
 * **************************************************************************************/

//enum DataType { DT_MAP, DT_F2DARRAY };

//! We choose for a typedef instead of generics, because else way <DC_TYPE> is used all over our code.
typedef double DC_TYPE;

/**
 * Internally the data is stored as DataItem%s and this is also how data is communicated to the plotting
 * functions.
 */
struct DataItem {
	int coord_x;
	int coord_y;
	DC_TYPE value;
};

typedef std::vector<DataItem*> DC_DATA_ITEMS;

/**
 * The idea of DataContainer was that it did not actually contain the data itself, but only references to the
 * data that is maintained outside of the container. However, it is normally used for plotting and it might be
 * cleaner to just provide different methods with which data can be added to the container, but have only one
 * method with which data can be obtained from the container. There are basically two ways in which things can
 * be plotted. One as a graph having an x and an y-coordinate for example. The other as a 2D plot with values
 * for each and every x and y pixel. In the case of a graph, the data is two-dimensional with a length "t".
 * In the case of a plot, the data is also two-dimensional with a width and a height.
 */
class DataContainer {
public:
	//! Constructor DataContainer
	DataContainer();

	//! Destructor ~DataContainer
	virtual ~DataContainer();

	//! Sets the data type
//	inline void SetType(DataType dataType) { this->dataType = dataType; }

//	//! Point towards data in the form of a map
//	inline void SetData(std::map<DC_TYPE,int> & data) { this->map_data = &data; dataType = DT_MAP; }
//
//	//! Point towards data in the form of an array
//	inline void SetData(float *data, int len) { float_data = data; float_data_len = len; dataType = DT_F2DARRAY; data_width = sqrt(float_data_len);}
//
//	inline void SetData(std::vector<float> &data) {
//		if (float_data == NULL)
//			float_data = new float[data.size()];
//
//		for (int i = 0; i < data.size(); ++i) {
//			float_data[i] = data[i];
//		}
//		float_data_len = data.size();
//		data_width = sqrt(float_data_len);
//		dataType = DT_F2DARRAY;
//	}
//
//	inline void SetData(std::vector<bool> &data) {
//		if (float_data == NULL)
//			float_data = new float[data.size()];
//
//		for (int i = 0; i < data.size(); ++i) {
//			data[i] ? float_data[i] = 125 : float_data[i] = 0;
//		}
//		float_data_len = data.size();
//		data_width = sqrt(float_data_len);
//		dataType = DT_F2DARRAY;
//	}

	//! Add a data item
	void addItem(DataItem *item);

	//! Add a series of items
	void addItems(std::map<DC_TYPE,int> & items);

	//! Add a series of items
	void addItems(std::vector<DC_TYPE> & items, int xcoord);

	//! Remove all items
	void clear();

	//! Get a data item
	DataItem * getItem(int index);

	//! Return number of data elements
	int size();

	//! Return width
	inline int height() { return data_height; }

	//! Read data from stream (can be a file)
	void read(std::istream& in);

	//! Write to file or stream
	void write(std::ostream& out);

	//! Calculate the slope in the loglog plot
	float CalculateSlope();

	//! Id can be used for identification purposes (e.g. in plotting)
	inline void SetID(int id) { this->id = id; }

	//! ID idem
	inline int GetID() { return id; }

	//! Apply bins to the data (in DT_MAP case)
//	void ApplyBins(int no_bins, DataDecoratorType min, DataDecoratorType max);
private:
	//! The identifier for this container
	int id;

	//! Internal structure of data
	DC_DATA_ITEMS data_items;

	//! Height of the to-be-created 2D map
	int data_height;
};

#endif /* DATADECORATOR_H_ */
