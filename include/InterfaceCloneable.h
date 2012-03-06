/**
 * ICloneable Interface Definition
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file InterfaceCloneable.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-10-23
 */


#ifndef INTERFACE_CLONEABLE_INCLUDE_H
#define INTERFACE_CLONEABLE_INCLUDE_H
//


/**
 * Description: For cloning the original object.
 */
class ICloneable
{
public:
	/**
	 * Description: Clone the original object.
	 * @return: A pointer to the clone object.
	 * @note: The returned pointer can be safely cast to any base pointer. It is the resposibility of the client to maitain the returned pointer.
	 */
	virtual ICloneable* clone() const = 0;

	virtual ~ICloneable(){};
};


//
#endif
