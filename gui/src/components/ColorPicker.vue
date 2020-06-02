<template>
  <div>
    <q-btn
      v-for="(color, index) in colors"
      :key="color"
      round
      :color="color"
      :style="{ backgroundColor: color }"
      class="button"
      @click="setColor(index)"
    />
  </div>
</template>

<script lang="ts">
import { defineComponent } from '@vue/composition-api'

const colors = [
  '#000000',
  '#ff0080',
  '#f11503',
  '#f47f81',
  '#f37e05',
  '#f38041',
  '#faff01',
  '#fbff7e',
  '#237f01',
  '#4fff00',
  '#61feff',
  '#81fdfc',
  '#3b00fe',
  '#807fff',
  '#807fc0',
  '#457eff',
  '#408080',
  '#2201a0',
  '#f600fe',
  '#f57ebe',
  '#ffffff',
  '#c0c0c0',
  '#7f00ff',
  '#f880fe'
]

export default defineComponent({
  name: 'CompositionComponent',
  props: {
    value: Number
  },
  setup (_, { emit }) {
    return {
      setColor: (index: number) => emit('input', index + 1),
      colors
    }
  }
})
</script>

<style lang="scss" scoped>
.button {
  border: 2px solid rgba(255,255,255, 0.3);
  margin: 2px;
}
</style>
