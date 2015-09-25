LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := hacksepol

#LOCAL_CFLAGS := -Wl,-whole-archive -l./obj/local/arm64-v8a/libsepol.a -Wl,-no-whole-archive
#LOCAL_LDFLAGS := -Wl,-whole-archive -Wl,-lsepol.a -Wl,-no-whole-archive

common_src_files := \
	libsepol/src/assertion.c \
	libsepol/src/avrule_block.c \
	libsepol/src/avtab.c \
	libsepol/src/boolean_record.c \
	libsepol/src/booleans.c \
	libsepol/src/conditional.c \
	libsepol/src/constraint.c \
	libsepol/src/context.c \
	libsepol/src/context_record.c \
	libsepol/src/debug.c \
	libsepol/src/ebitmap.c \
	libsepol/src/expand.c \
	libsepol/src/genbools.c \
	libsepol/src/genusers.c \
	libsepol/src/handle.c \
	libsepol/src/hashtab.c \
	libsepol/src/hierarchy.c \
	libsepol/src/iface_record.c \
	libsepol/src/interfaces.c \
	libsepol/src/link.c \
	libsepol/src/mls.c \
	libsepol/src/module.c \
	libsepol/src/node_record.c \
	libsepol/src/nodes.c \
	libsepol/src/polcaps.c \
	libsepol/src/policydb.c \
	libsepol/src/policydb_convert.c \
	libsepol/src/policydb_public.c \
	libsepol/src/port_record.c \
	libsepol/src/ports.c \
	libsepol/src/roles.c \
	libsepol/src/services.c \
	libsepol/src/sidtab.c \
	libsepol/src/symtab.c \
	libsepol/src/user_record.c \
	libsepol/src/users.c \
	libsepol/src/util.c \
	libsepol/src/write.c

common_includes := \
	$(LOCAL_PATH)/libsepol/src

LOCAL_CFLAGS := -fvisibility=hidden

LOCAL_C_INCLUDES := \
	$(common_includes) \
	$(LOCAL_PATH)/libsepol/include


LOCAL_SRC_FILES := \
	$(common_src_files) \
	hacksepol/main.c

include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := seinfo
LOCAL_CFLAGS :=
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/libsepol/include \
	$(LOCAL_PATH)/include

LOCAL_SRC_FILES := \
	secmds/seinfo.c

LOCAL_STATIC_LIBRARIES := libapol libqpol libsepol

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := sesearch
LOCAL_CFLAGS := -std=gnu99
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/libsepol/include \
	$(LOCAL_PATH)/include

LOCAL_SRC_FILES := \
	secmds/sesearch.c

LOCAL_STATIC_LIBRARIES := libapol libqpol libsepol

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := sepolicy-inject
LOCAL_CFLAGS :=
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/libsepol/include

LOCAL_SRC_FILES := \
	sepolicy-inject/sepolicy-inject.c

LOCAL_STATIC_LIBRARIES := libsepol

include $(BUILD_EXECUTABLE)

$(call import-add-path, $(LOCAL_PATH))
$(call import-module, libapol)
$(call import-module, libqpol)
$(call import-module, libsepol)
