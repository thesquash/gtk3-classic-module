#ifndef __GTK3_CLASSIC_H__
#define __GTK3_CLASSIC_H__


#define GTK_GETTEXT_DOMAIN	"gtk30"
#include <glib/gi18n.h>


#define INTERCEPTED_CLASS_METHOD(type_name, method, parameters, return_type) \
 static return_type (* original_##type_name##_##method)  parameters; \
 static return_type new_##type_name##_##method  parameters;

#define INTERCEPT_CLASS_METHOD(type_name, class_cast, method) \
 original_##type_name##_##method = class_cast (type_name##_class)->method; \
 class_cast (type_name##_class)->method = new_##type_name##_##method;

#define CALL_ORIGINAL_CLASS_METHOD(type_name, method, parameters) \
 if (original_##type_name##_##method) \
   original_##type_name##_##method parameters;

#define CALL_ORIGINAL_CLASS_METHOD_UNCHECKED(type_name, method, parameters) \
 original_##type_name##_##method parameters;


#endif /* __GTK3_CLASSIC_H__ */
