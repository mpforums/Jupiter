/**
 * Copyright (C) 2015 Justin James.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Written by Justin James <justin.aj@hotmail.com>
 */

#if !defined _RENX_NICKNAMEUUID_H_HEADER
#define _RENX_NICKNAMEUUID_H_HEADER

#include "Jupiter/Plugin.h"
#include "Jupiter/Reference_String.h"
#include "RenX_Plugin.h"

class RenX_NicknameUUIDPlugin : public RenX::Plugin
{
public: // RenX::Plugin
	void RenX_OnServerCreate(RenX::Server *server) override;

public: // Jupiter::Plugin
	const Jupiter::ReadableString &getName() override { return name; }

	RenX_NicknameUUIDPlugin();
	~RenX_NicknameUUIDPlugin();

private:
	STRING_LITERAL_AS_NAMED_REFERENCE(name, "RenX.NicknameUUID");
};

#endif // _RENX_NICKNAMEUUID_H_HEADER