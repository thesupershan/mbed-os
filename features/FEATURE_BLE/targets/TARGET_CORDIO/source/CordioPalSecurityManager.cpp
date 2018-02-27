/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "CordioPalSecurityManager.h"
#include "dm_api.h"
#include "smp_api.h"
#include "wsf_os.h"

namespace ble {
namespace pal {
namespace vendor {
namespace cordio {

CordioSecurityManager::CordioSecurityManager() :
    ::ble::pal::SecurityManager(),
    _use_default_passkey(false),
    _default_passkey(0)
{

}

CordioSecurityManager::~CordioSecurityManager()
{

}

////////////////////////////////////////////////////////////////////////////
// SM lifecycle management
//

ble_error_t CordioSecurityManager::initialize()
{
    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::terminate()
{
    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::reset()
{
    return BLE_ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////
// Resolving list management
//

uint8_t CordioSecurityManager::read_resolving_list_capacity()
{
    // FIXME: implement
    return 0;
}

ble_error_t CordioSecurityManager::add_device_to_resolving_list(
    advertising_peer_address_type_t peer_identity_address_type,
    const address_t &peer_identity_address,
    const irk_t &peer_irk
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::remove_device_from_resolving_list(
    advertising_peer_address_type_t peer_identity_address_type,
    const address_t& peer_identity_address
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::clear_resolving_list()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Feature support
//

ble_error_t CordioSecurityManager::set_secure_connections_support(
    bool enabled, bool secure_connections_only
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::get_secure_connections_support(
    bool &enabled
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Security settings
//

ble_error_t CordioSecurityManager::set_authentication_timeout(
    connection_handle_t, uint16_t timeout_in_10ms
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::get_authentication_timeout(
    connection_handle_t, uint16_t &timeout_in_10ms
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::slave_security_request(
    connection_handle_t connection,
    AuthenticationMask authentication
) {
    DmSecSlaveReq(connection, authentication.value());
    return BLE_ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////
// Encryption
//

ble_error_t CordioSecurityManager::enable_encryption(
    connection_handle_t connection,
    const ltk_t &ltk,
    const rand_t &rand,
    const ediv_t &ediv
) {
    dmSecLtk_t sec_ltk;
    memcpy(sec_ltk.key, ltk.data(), ltk.size());
    memcpy(sec_ltk.rand, rand.data(), rand.size());
    memcpy(&sec_ltk.ediv, ediv.data(), ediv.size());

    DmSecEncryptReq(
        connection,
        /* FIXME: Security Level */ DM_SEC_LEVEL_ENC_AUTH,
        &sec_ltk
    );

    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::enable_encryption(
    connection_handle_t connection,
    const ltk_t &ltk
) {
    dmSecLtk_t sec_ltk = { 0 };
    memcpy(sec_ltk.key, ltk.data(), ltk.size());

    DmSecEncryptReq(
        connection,
        DM_SEC_LEVEL_ENC_LESC,
        &sec_ltk
    );


    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::disable_encryption(connection_handle_t connection)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::get_encryption_status(
    connection_handle_t connection, LinkSecurityStatus_t &status
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::get_encryption_key_size(
    connection_handle_t, uint8_t &bitsize
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::encrypt_data(
    const key_t &key,
    encryption_block_t &data
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Privacy
//

ble_error_t CordioSecurityManager::set_private_address_timeout(
    uint16_t timeout_in_seconds
) {
    DmPrivSetResolvablePrivateAddrTimeout(timeout_in_seconds);
    return BLE_ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////
// Keys
//

ble_error_t CordioSecurityManager::set_ltk(
    connection_handle_t connection, const ltk_t& ltk
) {
    // FIXME: get access to the security level of a key
    DmSecLtkRsp(
        connection,
        /* key found */ true,
        /* sec level ??? */ DM_SEC_LEVEL_ENC_AUTH,
        const_cast<uint8_t*>(ltk.data())
    );
    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::set_ltk_not_found(
    connection_handle_t connection
) {
    DmSecLtkRsp(
        connection,
        /* key found */ false,
        /* sec level ??? */ DM_SEC_LEVEL_NONE,
        NULL
    );

    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::set_irk(const irk_t& irk)
{
    DmSecSetLocalIrk(const_cast<uint8_t*>(irk.data()));
    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::set_csrk(const csrk_t& csrk)
{
    DmSecSetLocalCsrk(const_cast<uint8_t*>(csrk.data()));
    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::generate_public_key()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Global parameters
//

ble_error_t CordioSecurityManager::set_display_passkey(passkey_num_t passkey)
{
    if (passkey) {
        _use_default_passkey = true;
        _default_passkey = passkey;
    } else {
        _use_default_passkey = false;
    }
    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::set_io_capability(io_capability_t io_capability)
{
    pSmpCfg->auth = io_capability.value();

    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::set_encryption_key_requirements(
    uint8_t min_encryption_key_size,
    uint8_t max_encryption_key_size
) {
    if ((min_encryption_key_size < 7) || (min_encryption_key_size > 16) ||
        (min_encryption_key_size > max_encryption_key_size)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    pSmpCfg->minKeyLen = min_encryption_key_size;
    pSmpCfg->maxKeyLen = max_encryption_key_size;

    return BLE_ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////
// Authentication
//

ble_error_t CordioSecurityManager::send_pairing_request(
    connection_handle_t connection,
    bool oob_data_flag,
    AuthenticationMask authentication_requirements,
    KeyDistribution initiator_dist,
    KeyDistribution responder_dist
) {
    //FIXME: understand if this is required
    pSmpCfg->auth = authentication_requirements.value();

    DmSecPairReq(
        connection,
        oob_data_flag,
        authentication_requirements.value(),
        initiator_dist.value(),
        responder_dist.value()
    );

    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::send_pairing_response(
    connection_handle_t connection,
    bool oob_data_flag,
    AuthenticationMask authentication_requirements,
    KeyDistribution initiator_dist,
    KeyDistribution responder_dist
) {
    DmSecPairRsp(
        connection,
        oob_data_flag,
        authentication_requirements.value(),
        initiator_dist.value(),
        responder_dist.value()
    );

    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::cancel_pairing(
    connection_handle_t connection, pairing_failure_t reason
) {
    DmSecCancelReq(connection, reason.value());
    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::request_authentication(connection_handle_t connection)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::get_random_data(octet_type_t<8> &random_data)
{
    SecRand(random_data.buffer(), random_data.size());
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// MITM
//

ble_error_t CordioSecurityManager::passkey_request_reply(
    connection_handle_t connection, passkey_num_t passkey
) {
    DmSecAuthRsp(
        connection,
        3,
        reinterpret_cast<uint8_t*>(&passkey)
    );

    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::legacy_pairing_oob_data_request_reply(
    connection_handle_t connection,
    const oob_tk_t &oob_data
) {
    DmSecAuthRsp(
        connection,
        16,
        const_cast<uint8_t*>(oob_data.data())
    );

    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::confirmation_entered(
    connection_handle_t connection, bool confirmation
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::send_keypress_notification(
    connection_handle_t connection, Keypress_t keypress
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::oob_data_verified(
    connection_handle_t connection,
    const oob_lesc_value_t &local_random,
    const oob_lesc_value_t &peer_random
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

CordioSecurityManager& CordioSecurityManager::get_security_manager()
{
    static CordioSecurityManager _security_manager;
    return _security_manager;
}

bool CordioSecurityManager::sm_handler(const wsfMsgHdr_t* msg) {
    SecurityManagerEventHandler* handler =
        get_security_manager().get_event_handler();

    if ((msg == NULL) || (handler == NULL)) {
        return false;
    }

    switch (msg->event) {
        case DM_SEC_PAIR_CMPL_IND: {
            dmSecPairCmplIndEvt_t* evt = (dmSecPairCmplIndEvt_t*) msg;
            // Note: authentication and bonding flags present in the auth field
            handler->on_pairing_completed(evt->hdr.param);
            return true;
        }

        case DM_SEC_PAIR_FAIL_IND: {
            connection_handle_t connection = msg->param;
            uint8_t status = msg->status;

            if (status >= pairing_failure_t::PASSKEY_ENTRY_FAILED &&
                status <= pairing_failure_t::CROSS_TRANSPORT_KEY_DERIVATION_OR_GENERATION_NOT_ALLOWED) {
                handler->on_pairing_error(
                    connection,
                    (pairing_failure_t::type) status
                );
            } else if (status == SMP_ERR_MEMORY) {
                // note: report the error as an unspecified error
                handler->on_pairing_error(
                    connection,
                    pairing_failure_t::UNSPECIFIED_REASON
                );
            } else if (msg->status == SMP_ERR_TIMEOUT) {
                handler->on_pairing_timed_out(connection);
            } else {
                // note: report the error as an unspecified error
                handler->on_pairing_error(
                    connection,
                    pairing_failure_t::UNSPECIFIED_REASON
                );
            }
            return true;
        }

        case DM_SEC_ENCRYPT_IND: {
            dmSecEncryptIndEvt_t* evt = (dmSecEncryptIndEvt_t*) msg;
            // note: the field usingLtk of the message indicates if an LTK was
            // used to encrypt the link
            handler->on_link_encryption_result(evt->hdr.param, link_encryption_t::ENCRYPTED);
            return true;
        }

        case DM_SEC_ENCRYPT_FAIL_IND: {
            // note: msg->status contains the encryption failure status
            handler->on_link_encryption_result(msg->param, link_encryption_t::NOT_ENCRYPTED);
            return true;
        }

        case DM_SEC_AUTH_REQ_IND: {
            dmSecAuthReqIndEvt_t* evt = (dmSecAuthReqIndEvt_t*) msg;
            connection_handle_t connection = evt->hdr.param;

            if (evt->oob) {
                handler->on_legacy_pairing_oob_request(connection);
            } else if (evt->display) {
                if (get_security_manager()._use_default_passkey) {
                    handler->on_passkey_display(
                        connection,
                        get_security_manager()._default_passkey
                    );
                    DmSecAuthRsp(
                        connection,
                        3,
                        reinterpret_cast<uint8_t*>(&(get_security_manager()._default_passkey))
                    );
                } else {
                    // FIXME: generate a random passkey
                    passkey_num_t passkey = 0x00654321;
                    handler->on_passkey_display(connection, passkey);
                    DmSecAuthRsp(
                        connection, 3, reinterpret_cast<uint8_t*>(&passkey)
                    );
                }
            } else {
                handler->on_passkey_request(connection);
            }
            return true;
        }

        case DM_SEC_KEY_IND: {
            dmSecKeyIndEvt_t* evt = (dmSecKeyIndEvt_t*) msg;
            connection_handle_t connection = evt->hdr.param;

            switch(evt->type) {
                case DM_KEY_LOCAL_LTK:
                    // FIXME: forward local ltk
#if 0
                    printf("local ltk: ");
                    for (size_t i = 0; i < sizeof(ltk_t); ++i) {
                        printf("%02X ", evt->keyData.ltk.key[i]);
                    }
                    printf("\r\n");

                    printf("local ediv: %04X\r\n", evt->keyData.ltk.ediv);
                    printf("local rand: ");
                    for (size_t i = 0; i < sizeof(rand_t); ++i) {
                        printf("%02X ", evt->keyData.ltk.rand[i]);
                    }
                    printf("\r\n");
                    memcpy(local_ltk, evt->keyData.ltk.key, sizeof(local_ltk));
#endif
                    break;

                case DM_KEY_PEER_LTK:
                    handler->on_keys_distributed_ltk(
                        connection,
                        ltk_t(reinterpret_cast<uint8_t*>(evt->keyData.ltk.key))
                    );
                    handler->on_keys_distributed_ediv_rand(
                        connection,
                        ediv_t(reinterpret_cast<uint8_t*>(&(evt->keyData.ltk.ediv))),
                        evt->keyData.ltk.rand
                    );
                    break;

                case DM_KEY_IRK:
                    handler->on_keys_distributed_bdaddr(
                        connection,
                        (advertising_peer_address_type_t::type) evt->keyData.irk.addrType,
                        evt->keyData.irk.bdAddr
                    );
                    handler->on_keys_distributed_irk(
                        connection,
                        irk_t(reinterpret_cast<uint8_t*>(evt->keyData.irk.key))
                    );
                    break;

                case DM_KEY_CSRK:
                    handler->on_keys_distributed_csrk(
                        connection, evt->keyData.csrk.key
                    );
                    break;
            }

            // TODO: what to do with the security level and encryption key len ???

            return true;
        }

        case DM_SEC_LTK_REQ_IND: {
            hciLeLtkReqEvt_t* evt = (hciLeLtkReqEvt_t*) msg;
            handler->on_ltk_request(
                evt->hdr.param,
                reinterpret_cast<uint8_t*>(&evt->encDiversifier),
                evt->randNum
            );
            return true;
        }

        case DM_SEC_PAIR_IND: {
            dmSecPairIndEvt_t* evt = (dmSecPairIndEvt_t*) msg;
            handler->on_pairing_request(
                /* connection */ evt->hdr.param,
                evt->oob,
                evt->auth,
                evt->iKeyDist,
                evt->rKeyDist
            );
            return true;
        }

        case DM_SEC_SLAVE_REQ_IND:
            return true;

        case DM_SEC_CALC_OOB_IND:
            return true;

        case DM_SEC_ECC_KEY_IND:
            return true;

        case DM_SEC_COMPARE_IND:
            return true;

        case DM_SEC_KEYPRESS_IND:
            return true;

        default:
            return false;
    }
}


} // cordio
} // vendor
} // pal
} // ble

