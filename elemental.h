/****************************************************************************************
	elemental.h
	
	A reenterant, intrusive, fast (constant-speed) list.
	
	Copyright © 1999-2006 Red Shed Software. All rights reserved.
	by Jonathan 'Wolf' Rentzsch (jon@redshed.net)
	
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.
	wolf		Mon, May 3, 1999	Added ___Type macros.
	wolf		Wed, May 31, 2000	Now each element stores a pointer to its list.
									Added GetElementList[Off|Type]() and IsListEmpty().
	wolf		Tue, Feb 27, 2001	CodeWarrior Pro 6 breaks an idiom: implicit
									casting of <type>** to void**. I had to make all
									such casts explicit.
	
	************************************************************************************/

#ifndef		_elemental_
#define		_elemental_

#include <stddef.h>

#ifdef	__cplusplus
	extern "C" {
#else
	typedef int bool;
#endif

/**************************
*	
*	Types
*	
**************************/
#pragma mark	(Types)

typedef	struct	Element		Element;
typedef	struct	ElementList	ElementList;

struct	Element	{
	Element		*next;
	Element		*prev;
	ElementList	*list;
	
#ifdef	__cplusplus
	Element() : next( NULL ), prev( NULL ), list( NULL ){}
#endif
};

struct	ElementList	{
	Element	*first;
	Element	*last;
	
#ifdef	__cplusplus
	ElementList() { NewElementList(this); }
	~ElementList() { DeleteElementList(this); }
#endif
};

/**************************
*	
*	Lifetime
*	
**************************/
#pragma mark	-
#pragma mark	(Lifetime)

	extern
	void
NewElementList(
	ElementList	*list );

	extern
	void
DeleteElementList(
	ElementList	*list );

/**************************
*	
*	Putters
*	
**************************/
#pragma mark	-
#pragma mark	(Putters)

//	If list == a, b, c && element == x
//	Then list = x, a, b, c
	extern
	void
PutFirstElement(
	void			*element,
	ElementList		*list );
	
//	If list == a, b, c && element == x
//	Then list = a, b, c, x
	extern
	void
PutLastElement(
	void			*element,
	ElementList		*list );

//	If list == a, b, c && element == x && before == b
//	Then list = a, x, b, c
//	Special Case: if before == NULL then PutLastElement( element )
	extern
	void
PutBeforeElement(
	void			*element,
	void			*before,
	ElementList		*list );

//	If list == a, b, c && element == x && after == b
//	Then list = a, b, x, c
//	Special Case: if after == NULL then PutFirstElement( element )
	extern
	void
PutAfterElement(
	void			*element,
	void			*after,
	ElementList		*list );

/**************************
*	
*	Accessors
*	
**************************/
#pragma mark	-
#pragma mark	(Accessors)

//	If list == a, b, c
//	Then *element = a
	extern
	void
FirstElement(
	void			**element,
	ElementList		*list );

//	If list == a, b, c
//	Then *element = c
	extern
	void
LastElement(
	void			**element,
	ElementList		*list );

//	If list == a, b, c && element == b
//	Then *nextElement = c
	extern
	void
NextElement(
	void		*element,
	void		**nextElement );

//	If list == a, b, c && element == b
//	Then *prevElement = a
	extern
	void
PrevElement(
	void		*element,
	void		**prevElement );

//	Return whether element is in list. The only linear-time function here.
	extern
	bool
FindElement(
	void			*element,
	ElementList		*list );

//	Returns the given element's list.
	extern
	ElementList*
GetElementList(
	void	*element );
	
//	Returns whether the list is empty.
	extern
	bool
IsListEmpty(
	ElementList	*list );

/**************************
*	
*	Grabbing
*	
**************************/
#pragma mark	-
#pragma mark	(Grabbing)

//	If list == a, b, c && element == b
//	Then list = a, c
//	It is okay if element is not in list.
	extern
	void
RemoveElement(
	void			*element,
	ElementList		*list );

//	If list == a, b, c
//	Then list = b, c && *element = a
	extern
	void
GrabFirstElement(
	void			**element,
	ElementList		*list );

//	If list == a, b, c
//	Then list = a, b && *element = c
	extern
	void
GrabLastElement(
	void			**element,
	ElementList		*list );

//	If list == a, b, c && element == b
//	Then list = a, c && *nextElement = c
	extern
	void
GrabNextElement(
	void			*element,
	void			**nextElement,
	ElementList		*list );

//	If list == a, b, c && element == b
//	Then list = a, c && *prevElement = a
	extern
	void
GrabPrevElement(
	void			*element,
	void			**prevElement,
	ElementList		*list );

/**************************
*	
*	Offset Putters
*	
**************************/
#pragma mark	-
#pragma mark	(Offset Putters)

//	If list == a, b, c && element == x
//	Then list = x, a, b, c
	extern
	void
PutFirstElementOff(
	void			*element,
	ElementList		*list,
	size_t			offset );
	
//	If list == a, b, c && element == x
//	Then list = a, b, c, x
	extern
	void
PutLastElementOff(
	void			*element,
	ElementList		*list,
	size_t			offset );

//	If list == a, b, c && element == x && before == b
//	Then list = a, x, b, c
//	Special Case: if before == NULL then PutLastElement( element )
	extern
	void
PutBeforeElementOff(
	void			*element,
	void			*before,
	ElementList		*list,
	size_t			offset );

//	If list == a, b, c && element == x && after == b
//	Then list = a, b, x, c
//	Special Case: if after == NULL then PutFirstElement( element )
	extern
	void
PutAfterElementOff(
	void			*element,
	void			*after,
	ElementList		*list,
	size_t			offset );

/**************************
*	
*	Offset Accessors
*	
**************************/
#pragma mark	-
#pragma mark	(Offset Accessors)

//	If list == a, b, c
//	Then *element = a
	extern
	void
FirstElementOff(
	void			**element,
	ElementList		*list,
	size_t			offset );

//	If list == a, b, c
//	Then *element = c
	extern
	void
LastElementOff(
	void			**element,
	ElementList		*list,
	size_t			offset );

//	If list == a, b, c && element == b
//	Then *nextElement = c
	extern
	void
NextElementOff(
	void		*element,
	void		**nextElement,
	size_t		offset );

//	If list == a, b, c && element == b
//	Then *prevElement = a
	extern
	void
PrevElementOff(
	void		*element,
	void		**prevElement,
	size_t		offset );

//	Return whether element is in list. The only linear-time function here.
	extern
	bool
FindElementOff(
	void			*element,
	ElementList		*list,
	size_t			offset );

//	Returns the given element's list.
	extern
	ElementList*
GetElementListOff(
	void	*element,
	size_t	offset );

/**************************
*	
*	Offset Grabbing
*	
**************************/
#pragma mark	-
#pragma mark	(Offset Grabbing)

//	If list == a, b, c && element == b
//	Then list = a, c
//	It is okay if element is not in list.
	extern
	void
RemoveElementOff(
	void			*element,
	ElementList		*list,
	size_t			offset );

//	If list == a, b, c
//	Then list = b, c && *element = a
	extern
	void
GrabFirstElementOff(
	void			**element,
	ElementList		*list,
	size_t			offset );

//	If list == a, b, c
//	Then list = a, b && *element = c
	extern
	void
GrabLastElementOff(
	void			**element,
	ElementList		*list,
	size_t			offset );

//	If list == a, b, c && element == b
//	Then list = a, c && *nextElement = c
	extern
	void
GrabNextElementOff(
	void			*element,
	void			**nextElement,
	ElementList		*list,
	size_t			offset );

//	If list == a, b, c && element == b
//	Then list = a, c && *prevElement = a
	extern
	void
GrabPrevElementOff(
	void			*element,
	void			**prevElement,
	ElementList		*list,
	size_t			offset );

/**************************
*	
*	Type Putters
*	
**************************/
#pragma mark	-
#pragma mark	(Type Putters)

//	If list == a, b, c && element == x
//	Then list = x, a, b, c
#define	PutFirstElementType( ELEMENT, LIST, STRUCTURE, FIELD )	\
			PutFirstElementOff( (ELEMENT), (LIST), offsetof( STRUCTURE, FIELD ) )
	
//	If list == a, b, c && element == x
//	Then list = a, b, c, x
#define	PutLastElementType( ELEMENT, LIST, STRUCTURE, FIELD )	\
			PutLastElementOff( (ELEMENT), (LIST), offsetof( STRUCTURE, FIELD ) )

//	If list == a, b, c && element == x && before == b
//	Then list = a, x, b, c
//	Special Case: if before == NULL then PutLastElement( element )
#define	PutBeforeElementType( ELEMENT, BEFORE, LIST, STRUCTURE, FIELD )	\
			PutBeforeElementOff( (ELEMENT), (BEFORE), (LIST), offsetof( STRUCTURE, FIELD ) )

//	If list == a, b, c && element == x && after == b
//	Then list = a, b, x, c
//	Special Case: if after == NULL then PutFirstElement( element )
#define	PutAfterElementType( ELEMENT, AFTER, LIST, STRUCTURE, FIELD )	\
			PutAfterElementOff( (ELEMENT), (AFTER), (LIST), offsetof( STRUCTURE, FIELD ) )

/**************************
*	
*	Type Accessors
*	
**************************/
#pragma mark	-
#pragma mark	(Type Accessors)

//	If list == a, b, c
//	Then *element = a
#define	FirstElementType( ELEMENT, LIST, STRUCTURE, FIELD )	\
			FirstElementOff( (ELEMENT), (LIST), offsetof( STRUCTURE, FIELD ) )

//	If list == a, b, c
//	Then *element = c
#define	LastElementType( ELEMENT, LIST, STRUCTURE, FIELD )	\
			LastElementOff( (ELEMENT), (LIST), offsetof( STRUCTURE, FIELD ) )

//	If list == a, b, c && element == b
//	Then *nextElement = c
#define	NextElementType( ELEMENT, NEXTELELEMENT, STRUCTURE, FIELD )	\
			NextElementOff( (ELEMENT), (NEXTELELEMENT), offsetof( STRUCTURE, FIELD ) )

//	If list == a, b, c && element == b
//	Then *prevElement = a
#define	PrevElementType( ELEMENT, PREVELEMENT, STRUCTURE, FIELD )	\
			PrevElementOff( (ELEMENT), (PREVELEMENT), offsetof( STRUCTURE, FIELD ) )

//	Return whether element is in list. The only linear-time function here.
#define	FindElementType( ELEMENT, LIST, STRUCTURE, FIELD )	\
			FindElementOff( (ELEMENT), (LIST), offsetof( STRUCTURE, FIELD ) )

//	Returns the given element's list.
#define	GetElementListType( ELEMENT, STRUCTURE, FIELD )	\
			GetElementListOff( (ELEMENT), offsetof( STRUCTURE, FIELD ) )

/**************************
*	
*	Type Grabbing
*	
**************************/
#pragma mark	-
#pragma mark	(Type Grabbing)

//	If list == a, b, c && element == b
//	Then list = a, c
//	It is okay if element is not in list.
#define	RemoveElementType( ELEMENT, LIST, STRUCTURE, FIELD )	\
			RemoveElementOff( (ELEMENT), (LIST), offsetof( STRUCTURE, FIELD ) )

//	If list == a, b, c
//	Then list = b, c && *element = a
#define	GrabFirstElementType( ELEMENT, LIST, STRUCTURE, FIELD )	\
			GrabFirstElementOff( (ELEMENT), (LIST), offsetof( STRUCTURE, FIELD ) )

//	If list == a, b, c
//	Then list = a, b && *element = c
#define	GrabLastElementType( ELEMENT, LIST, STRUCTURE, FIELD )	\
			GrabLastElementOff( (ELEMENT), (LIST), offsetof( STRUCTURE, FIELD ) )

//	If list == a, b, c && element == b
//	Then list = a, c && *nextElement = c
#define	GrabNextElementType( ELEMENT, NEXTELEMENT, LIST, STRUCTURE, FIELD )	\
			GrabNextElementOff( (ELEMENT), (NEXTELEMENT), (LIST), offsetof( STRUCTURE, FIELD ) )

//	If list == a, b, c && element == b
//	Then list = a, c && *prevElement = a
#define	GrabPrevElementType( ELEMENT, PREVELEMENT, LIST, STRUCTURE, FIELD )	\
			GrabPrevElementOff( (ELEMENT), (PREVELEMENT), (LIST), offsetof( STRUCTURE, FIELD ) )

#ifdef	__cplusplus
}
#endif
#endif	//	_elemental_