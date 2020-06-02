<template>
  <q-page class="row items-center q-pt-md" background="">
    <q-btn
      class="quit-button"
      icon="power_settings_new"
      round
      flat
      @click="quit"
    />
    <q-list class="full-width">
      <q-item>
        <q-item-section avatar>
          <q-icon name="brightness_medium" />
        </q-item-section>

        <q-item-section>
          <q-btn-toggle
            v-model="intensity"
            class="button-toggle"
            rounded
            unelevated
            color="sh-inverted"
            text-color="primary"
            :options="[
              {label: 'off', value: 0},
              {label: 'low', value: 1},
              {label: 'middle', value: 2},
              {label: 'high', value: 3},
            ]"
          />
        </q-item-section>
      </q-item>

      <q-item>
        <q-item-section avatar>
          <q-icon name="waves" />
        </q-item-section>

        <q-item-section>
          <q-btn-toggle
            v-model="pulsation"
            class="button-toggle"
            rounded
            unelevated
            color="sh-inverted"
            text-color="primary"
            :options="[
              {label: 'steady', value: 0},
              {label: 'slow', value: 1},
              {label: 'middle', value: 2},
              {label: 'fast', value: 3},
            ]"
          />
        </q-item-section>
      </q-item>

      <q-item>
        <q-item-section avatar>
          <q-icon name="palette" />
        </q-item-section>
        <q-item-section>
          <ColorPicker v-model="color" />
        </q-item-section>
      </q-item>
    </q-list>
  </q-page>
</template>

<script lang="ts">
import { defineComponent, ref, watch } from '@vue/composition-api'
import { ipcRenderer, remote } from 'electron'
import ColorPicker from 'components/ColorPicker.vue'

export default defineComponent({
  name: 'CompositionComponent',
  components: {
    ColorPicker
  },
  setup () {
    const intensity = ref(1)
    const pulsation = ref(0)
    const color = ref(1)

    watch(() => [intensity.value, pulsation.value, color.value].join('|'), (_, oldValue) => {
      if (oldValue) {
        ipcRenderer.send('send-to-mouse', [intensity.value, pulsation.value, color.value])
      }
    })

    return {
      intensity,
      pulsation,
      color,
      quit: () => remote.app.quit()
    }
  }
})
</script>

<style lang="scss" scoped>
.button-toggle {
  align-self: flex-start;
  display: inline-block;
}

.body--light .button-toggle {
  border: 1px solid $primary;
}

.quit-button {
  position: absolute;
  top: 0;
  right: 0;
  margin: 5px 8px;
  z-index: 1;
}
</style>
