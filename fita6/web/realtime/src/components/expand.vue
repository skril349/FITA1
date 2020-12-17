<template>
  <div>
    <b-form-select v-model="selected" class="mb-3">
      <template v-slot:first>
        <b-form-select-option :value="null" disabled>-- Escull una planta --</b-form-select-option>
      </template>
      <b-form-select-option value="Rebanitos" disabled>Rabanitos</b-form-select-option>
      <b-form-select-option value="Patates" disabled>Patates</b-form-select-option>
      <b-form-select-option value="Menta">Menta</b-form-select-option>
      <b-form-select-option value="Meduixes" disabled>Meduixes</b-form-select-option>
      <b-form-select-option value="Enciam">Enciam</b-form-select-option>
      <b-form-select-option value="Pebrots">Pebrots</b-form-select-option>
      <b-form-select-option value="Rucula">Rúcula</b-form-select-option>
    </b-form-select>
    <div class="mt-3">
      Seleccionat:
      <strong>{{selected}}{{getData(selected)}}</strong>
    </div>
  </div>
</template>

<script>
import { mapState } from "vuex";

export default {
  name: 'Expand',
  props: {
    index: {
      type: Number
    }
  },
  data() {
    return {
      selected: null,
      valors: [],
      valuepos: 1
    };
  },
  methods: {
    getData(val) {
      this.$store.commit("getData", val);
      
    }
  }
};
</script>

<style>
</style>