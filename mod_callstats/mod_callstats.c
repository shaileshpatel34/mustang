/* 
 * FreeSWITCH Modular Media Switching Software Library / Soft-Switch Application
 * Copyright (C) 2005-2014, Anthony Minessale II <anthm@freeswitch.org>
 *
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is FreeSWITCH Modular Media Switching Software Library / Soft-Switch Application
 *
 * The Initial Developer of the Original Code is
 * Anthony Minessale II <anthm@freeswitch.org>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * 
 * Anthony Minessale II <anthm@freeswitch.org>
 *
 *
 * mod_callstats.c -- Framework Demo Module
 *
 */
#include <switch.h>

SWITCH_MODULE_LOAD_FUNCTION(mod_callstats_load);
SWITCH_MODULE_SHUTDOWN_FUNCTION(mod_callstats_shutdown);

SWITCH_MODULE_DEFINITION(mod_callstats, mod_callstats_load, mod_callstats_shutdown, NULL);

//#define TORTURE_ME

#define MY_EVENT_HANGUP "callstats::hangup"

static void event_handler(switch_event_t *event)
{
	char *buf;
	switch_xml_t xml;
	char *xmlstr = "N/A";
	uint8_t dofree = 0;
	switch_event_header_t *hi=NULL;
	switch_event_t *newevent=NULL;

	switch (event->event_id) {
	case SWITCH_EVENT_LOG:
		return;
	default:
		switch_event_serialize(event, &buf, SWITCH_TRUE);
		if ((xml = switch_event_xmlize(event, SWITCH_VA_NONE))) {
			xmlstr = switch_xml_toxml(xml, SWITCH_FALSE);
			dofree++;
		}

		/*
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_CONSOLE, "\nEVENT (text version)\n--------------------------------\n%s", buf);
		*/
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_CONSOLE, "\nEVENT (xml version)\n--------------------------------\n%s\n", xmlstr);
	

		if (switch_event_create_subclass(&newevent, SWITCH_EVENT_CUSTOM, MY_EVENT_HANGUP) == SWITCH_STATUS_SUCCESS) {
			hi = event->headers;
			while( hi != event->last_header ){
				/* switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_CONSOLE, "\n callstats %s : %s\n", (char *) hi->name,(char *) hi->value); */

				if (!strcasecmp((char *) hi->name, "Unique-ID")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "Unique-ID", "%s", (char *) hi->value);
				} else if (!strcasecmp((char *) hi->name, "Call-Direction")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "Direction", "%s", (char *) hi->value);
				} else if (!strcasecmp((char *) hi->name, "Caller-Channel-Created-Time")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "Start-Time", "%s", (char *) hi->value);
				} else if (!strcasecmp((char *) hi->name, "Caller-Channel-Progress-Time")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "Ring-Time", "%s", (char *) hi->value);
				} else if (!strcasecmp((char *) hi->name, "Caller-Channel-Progress-Media-Time")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "EarlyMedia-Time", "%s", (char *) hi->value);
				} else if (!strcasecmp((char *) hi->name, "Caller-Channel-Answered-Time")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "Answer-Time", "%s", (char *) hi->value);
				} else if (!strcasecmp((char *) hi->name, "Caller-Channel-Hangup-Time")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "Hangup-Time", "%s", (char *) hi->value);
				} else if (!strcasecmp((char *) hi->name, "variable_hangup_cause")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "Hangup-Cause", "%s", (char *) hi->value);
				} else if (!strcasecmp((char *) hi->name, "variable_rtp_audio_in_mos")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "MOS", "%s", (char *) hi->value);
				} else if (!strcasecmp((char *) hi->name, "variable_sip_local_network_addr")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "SIP-Local-IP", "%s", (char *) hi->value);
				} else if (!strcasecmp((char *) hi->name, "variable_sip_network_ip")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "SIP-Remote-IP", "%s", (char *) hi->value);
				}
				/*
				   - Post-Dial-Delay (ms - set if applicable else 0)
				   - SIP-BYE-From (the IP that sent the BYE. if no BYE empty string)
				   - SIP-BYE-Reason (if BYE and reason present in BYE packet, set here else empty string)
				   - SIP-Cancel-From (the IP that sent the CANCEL. if no CANCEL empty string)
				   - SIP-Failure-Response-Code (set if any SIP response >= 300 else empty string)
				   - SIP-Failure-Response-From (If SIP response >= 300, the IP that sent the SIP Response else empty string)
				 */
				else if (!strcasecmp((char *) hi->name, "variable_remote_media_ip")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "RTP-Remote-IP", "%s", (char *) hi->value);
				} else if (!strcasecmp((char *) hi->name, "variable_local_media_ip")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "RTP-Local-IP", "%s", (char *) hi->value);
				} else if (!strcasecmp((char *) hi->name, "variable_rtp_audio_in_raw_bytes")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "RTP-In-Bytes", "%s", (char *) hi->value);
				} else if (!strcasecmp((char *) hi->name, "variable_rtp_last_audio_codec_string")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "RTP-In-Codec", "%s", (char *) hi->value);
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "RTP-Out-Codec", "%s", (char *) hi->value);
				}
				/*
				   - RTP-In-Packet-Loss (%)
				   - RTP-In-Mean-Jitter (ms)
				   - RTP-In-Max-Jitter (ms)
				   - RTP-In-Avg-Latency (ms)
				   - RTP-In-Max-Latency (ms)
				 */
				else if (!strcasecmp((char *) hi->name, "variable_rtp_audio_out_raw_bytes")) {
					switch_event_add_header(newevent, SWITCH_STACK_BOTTOM, "RTP-Out-Bytes", "%s", (char *) hi->value);
				}
				/*			
							- RTP-Out-Packet-Loss (%)
							- RTP-Out-Mean-Jitter (ms)
							- RTP-Out-Max-Jitter (ms)
							- RTP-Out-Avg-Latency (ms)
							- RTP-Out-Max-Latency (ms)
				 */
				hi = hi->next;
			}
			switch_event_fire(&newevent);
		}
		break;
	}

	switch_safe_free(buf);

	if (dofree) {
		if (xml) {
			switch_xml_free(xml);
		}
		if (xmlstr) {
			free(xmlstr);
		}
	}
}


#ifdef TORTURE_ME
#define TTHREADS 500
static int THREADS = 0;

static void *torture_thread(switch_thread_t *thread, void *obj)
{
	int y = 0;
	int z = 0;
	switch_core_thread_session_t *ts = obj;
	switch_event_t *event;

	z = THREADS++;

	while (THREADS > 0) {
		int x;
		for (x = 0; x < 1; x++) {
			if (switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, MY_EVENT_HANGUP) == SWITCH_STATUS_SUCCESS) {
				switch_event_add_header(event, "event_info", "hello world %d %d", z, y++);
				switch_event_fire(&event);
			}
		}
		switch_yield(100000);
	}

	if (ts->pool) {
		switch_memory_pool_t *pool = ts->pool;
		switch_core_destroy_memory_pool(&pool);
	}

	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "Thread Ended\n");
}

SWITCH_MOD_DECLARE(switch_status_t) switch_module_shutdown(void)
{
	THREADS = -1;
	switch_yield(100000);
	return SWITCH_STATUS_SUCCESS;
}
#endif

SWITCH_MODULE_LOAD_FUNCTION(mod_callstats_load)
{
	/* connect my internal structure to the blank pointer passed to me */
	*module_interface = switch_loadable_module_create_module_interface(pool, modname);

	if (switch_event_reserve_subclass(MY_EVENT_HANGUP) != SWITCH_STATUS_SUCCESS) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Couldn't register subclass!");
		return SWITCH_STATUS_GENERR;
	}

	if (switch_event_bind(modname, SWITCH_EVENT_CHANNEL_HANGUP_COMPLETE, SWITCH_EVENT_SUBCLASS_ANY, event_handler, NULL) != SWITCH_STATUS_SUCCESS) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Couldn't bind!\n");
		return SWITCH_STATUS_GENERR;
	}
#ifdef TORTURE_ME
	if (1) {
		int x = 0;
		for (x = 0; x < TTHREADS; x++) {
			switch_core_launch_thread(torture_thread, NULL);
		}
	}
#endif

	/* indicate that the module should continue to be loaded */
	return SWITCH_STATUS_SUCCESS;
}

SWITCH_MODULE_SHUTDOWN_FUNCTION(mod_callstats_shutdown)
{
	switch_event_free_subclass(MY_EVENT_HANGUP);
        return SWITCH_STATUS_SUCCESS;
}
/* For Emacs:
 * Local Variables:
 * mode:c
 * indent-tabs-mode:t
 * tab-width:4
 * c-basic-offset:4
 * End:
 * For VIM:
 * vim:set softtabstop=4 shiftwidth=4 tabstop=4 noet:
 */
