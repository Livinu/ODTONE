//
// Copyright (c) 2007-2009 2009 Universidade Aveiro - Instituto de
// Telecomunicacoes Polo Aveiro
// This file is part of ODTONE - Open Dot Twenty One.
//
// This software is distributed under a license. The full license
// agreement can be found in the file LICENSE in this distribution.
// This software may not be copied, modified, sold or distributed
// other than expressed in the named license agreement.
//
// This software is distributed without any warranty.
//
// Author:     Simao Reis <sreis@av.it.pt>
//

#include <odtone/mihf/service_management.hpp>

#include <odtone/mihf/log.hpp>
#include <odtone/mihf/utils.hpp>
#include <odtone/mih/types/capabilities.hpp>
#include <odtone/mihf/local_transactions.hpp>
#include <odtone/mihf/mihfid.hpp>
#include <odtone/mihf/transmit.hpp>
#include <odtone/mih/request.hpp>
#include <odtone/mih/response.hpp>
#include <odtone/mih/tlv_types.hpp>

namespace odtone { namespace mihf {

bool service_management::capability_discover_request(mih::message_ptr& in,
													 mih::message_ptr& out)
{
	log(1, "(mism) received Capability_Discover.request from ", in->source().to_string());
	log(2, "(mism) received Capability_Discover.request with destination ", in->destination().to_string());

	if (utils::is_local_request(in))
		{
			log(2, "(mism) local request");

			*out << mih::response(mih::response::capability_discover)
				& mih::tlv_status(mih::status_success)
				& mih::tlv_net_type_addr_list(capabilities_list_net_type_addr)
				& mih::tlv_event_list(capabilities_event_list);

			out->tid(in->tid());
			out->destination(in->source());
			out->source(mihfid);

			return true;
		}
	else
		{
			log(2, "(mism) remote request");
			return utils::forward_request(in);
		}

	return false;
}

bool service_management::capability_discover_response(mih::message_ptr& in,
													  mih::message_ptr& /*out*/)
{
	log(1, "received Capability_Discover.response from ", in->source().to_string());

	// do we have a request from a user?
	pending_transaction_t p;
	mih::octet_string	  from = in->source().to_string();

	if (!local_transactions->get(from, p))
		{
			log(1, "no pending transaction for this message, discarding");
			return false;
		}

	log(1, "forwarding Capability_Discover.response to ", p.user);

	in->tid(p.tid);
	in->destination(mih::id(p.user));

	transmit(in);

	return false;
}


} /* namespace mihf */ } /* namespace odtone */