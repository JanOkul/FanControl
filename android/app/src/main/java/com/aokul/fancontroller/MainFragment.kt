package com.aokul.fancontroller

import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.lifecycle.lifecycleScope
import androidx.preference.PreferenceManager
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.IOException
import java.net.HttpURLConnection
import java.net.MalformedURLException
import java.net.SocketTimeoutException
import java.net.URL

/**
 * Enum of button names.
 */
enum class ButtonNames(val idName: String) {
    POWER("power"),
    SPEED_UP("speed up"),
    SPEED_DOWN("speed down"),
    TIMER_UP("timer up"),
    TIMER_DOWN("timer down"),
    SWING_UP("swing up"),
    SWING_SIDE("swing side")
}

/**
 *  The main page fragment for the fan controller app.
 */
class MainFragment : Fragment() {
    private lateinit var powerButton: Button
    private lateinit var speedUpButton: Button
    private lateinit var speedDownButton: Button
    private lateinit var timerUpButton: Button
    private lateinit var timerDownButton: Button

    private lateinit var swingUpButton: Button
    private lateinit var swingSideButton: Button
    private val buttonURLStateMap = hashMapOf<String, Boolean>() // Map of whether buttons have been given a URL in settings.

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_main, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        val sharedPreferences = PreferenceManager.getDefaultSharedPreferences(requireContext()) // Urls for server

        // Identify all the controller buttons
        powerButton = view.findViewById<Button>(R.id.button)
        speedDownButton = view.findViewById<Button>(R.id.button2)
        speedUpButton = view.findViewById<Button>(R.id.button3)
        timerDownButton = view.findViewById<Button>(R.id.button4)
        timerUpButton = view.findViewById<Button>(R.id.button5)
        swingSideButton = view.findViewById<Button>(R.id.button6)
        swingUpButton = view.findViewById<Button>(R.id.button7)

        // State of whether buttons have been assigned a URL or not.
        buttonURLStateMap[ButtonNames.POWER.name] = sharedPreferences.getString("power_path", "") != ""
        buttonURLStateMap[ButtonNames.SPEED_UP.name] = sharedPreferences.getString("speed_up_path", "") != ""
        buttonURLStateMap[ButtonNames.SPEED_DOWN.name] =
            sharedPreferences.getString("speed_down_path", "") != ""
        buttonURLStateMap[ButtonNames.TIMER_UP.name] = sharedPreferences.getString("timer_up_path", "") != ""
        buttonURLStateMap[ButtonNames.TIMER_DOWN.name] =
            sharedPreferences.getString("timer_down_path", "") != ""
        buttonURLStateMap[ButtonNames.SWING_UP.name] = sharedPreferences.getString("swing_up_path", "") != ""
        buttonURLStateMap[ButtonNames.SWING_SIDE.name] =
            sharedPreferences.getString("swing_side_path", "") != ""

        changeButtonState(true)


        // Set up button listeners
        powerButton.setOnClickListener {
            lifecycleScope.launch(Dispatchers.IO) {
                withContext(Dispatchers.Main) { changeButtonState(false) }
                sendFanCommand(
                    sharedPreferences.getString("power_path", "") as String,
                    ButtonNames.POWER
                )
                withContext(Dispatchers.Main) { changeButtonState(true) }
            }
        }

        speedUpButton.setOnClickListener {
            lifecycleScope.launch(Dispatchers.IO) {
                withContext(Dispatchers.Main) { changeButtonState(false) }
                sendFanCommand(
                    sharedPreferences.getString("speed_up_path", "") as String,
                    ButtonNames.SPEED_UP
                )
                withContext(Dispatchers.Main) { changeButtonState(true) }
            }
        }

        speedDownButton.setOnClickListener {
            lifecycleScope.launch(Dispatchers.IO) {
                withContext(Dispatchers.Main) { changeButtonState(false) }
                sendFanCommand(
                    sharedPreferences.getString("speed_down_path", "") as String,
                    ButtonNames.SPEED_DOWN
                )
                withContext(Dispatchers.Main) { changeButtonState(true) }
            }
        }

        timerUpButton.setOnClickListener {
            lifecycleScope.launch(Dispatchers.IO) {
                withContext(Dispatchers.Main) { changeButtonState(false) }
                sendFanCommand(
                    sharedPreferences.getString("timer_up_path", "") as String,
                    ButtonNames.TIMER_UP
                )
                withContext(Dispatchers.Main) { changeButtonState(true) }
            }
        }

        timerDownButton.setOnClickListener {
            lifecycleScope.launch(Dispatchers.IO) {
                withContext(Dispatchers.Main) { changeButtonState(false) }
                sendFanCommand(
                    sharedPreferences.getString("timer_down_path", "") as String,
                    ButtonNames.TIMER_DOWN
                )
                withContext(Dispatchers.Main) { changeButtonState(true) }
            }
        }

        swingUpButton.setOnClickListener {
            lifecycleScope.launch(Dispatchers.IO) {
                withContext(Dispatchers.Main) { changeButtonState(false) }
                sendFanCommand(
                    sharedPreferences.getString("swing_up_path", "") as String,
                    ButtonNames.SWING_UP
                )
                withContext(Dispatchers.Main) { changeButtonState(true) }
            }
        }

        swingSideButton.setOnClickListener {
            lifecycleScope.launch(Dispatchers.IO) {
                withContext(Dispatchers.Main) { changeButtonState(false) }
                sendFanCommand(
                    sharedPreferences.getString("swing_side_path", "") as String,
                    ButtonNames.SWING_SIDE
                )
                withContext(Dispatchers.Main) { changeButtonState(true) }
            }
        }
    }

    /**
     * Sends a command to the fan controller.
     * @param urlString The URL to send the command to.
     * @param buttonName The name of the button that was pressed.
     */
    private suspend fun sendFanCommand(urlString: String, buttonName: ButtonNames) {
        val TAG = "Send Command"

        var connection: HttpURLConnection? = null
        val buttonNameString = buttonName.idName
        Log.d(TAG, "Sending command to: $urlString")

        try {
            val url = URL(urlString)

            connection = url.openConnection() as HttpURLConnection
            connection.connectTimeout = 2000
            connection.readTimeout = 2000

            connection.connect()

            if (connection.responseCode == 200) {
                Log.d(TAG, "Request: $urlString made successfully")
            } else {
                Log.w(TAG, "Request failed with response code: ${connection.responseCode}")
                withContext(Dispatchers.Main) {
                    Toast.makeText(
                        requireContext(),
                        "Invalid URL PATH for $buttonNameString button.",
                        Toast.LENGTH_LONG
                    ).show()
                }
            }

        } catch (e: MalformedURLException) {
            withContext(Dispatchers.Main) {
                Toast.makeText(
                    requireContext(),
                    "Invalid URL, check URL for $buttonNameString",
                    Toast.LENGTH_LONG
                ).show()
            }
            Log.e(TAG, "Malformed URL, set properly in settings: ${e.message}")

        } catch (e: SocketTimeoutException) {
            withContext(Dispatchers.Main) {
                Toast.makeText(
                    requireContext(),
                    "Controller not responding, check power or restart.",
                    Toast.LENGTH_LONG
                ).show()
            }
            Log.w(TAG, "Socket timed out: ${e.message}")

        } catch (e: IOException) {
            withContext(Dispatchers.Main) {
                Toast.makeText(
                    requireContext(),
                    "Could not connect to the internet, please try again later.",
                    Toast.LENGTH_LONG
                ).show()
            }
            Log.e(TAG, "IO exception occurred: ${e.message}")

        } finally {
            connection?.disconnect()
        }
    }

    /**
     * Changes if the buttons are enabled or disabled,
     * but only enables if the button has a URL assigned.
     * @param state The state to change the buttons to.
     */
    private fun changeButtonState(state: Boolean) {
        powerButton.isEnabled = state && buttonURLStateMap[ButtonNames.POWER.name] ?: false
        speedUpButton.isEnabled = state && buttonURLStateMap[ButtonNames.SPEED_UP.name] ?: false
        speedDownButton.isEnabled = state && buttonURLStateMap[ButtonNames.SPEED_DOWN.name] ?: false
        timerUpButton.isEnabled = state && buttonURLStateMap[ButtonNames.TIMER_UP.name] ?: false
        timerDownButton.isEnabled = state && buttonURLStateMap[ButtonNames.TIMER_DOWN.name] ?: false
        swingUpButton.isEnabled = state && buttonURLStateMap[ButtonNames.SWING_UP.name] ?: false
        swingSideButton.isEnabled = state && buttonURLStateMap[ButtonNames.SWING_SIDE.name] ?: false
    }
}

